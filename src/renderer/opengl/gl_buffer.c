#include "base.h"
#include "renderer.h"
#include "renderer/gl_renderer.h"

#include <glad/gl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct _gl_vertex_attribute {
	const char *name;
	AttributeFormat format;
	uint32_t location, offset;
} OpenGLVertexAttribute;

typedef struct {
	OpenGLVertexAttribute *attributes;
	uint32_t count, stride;
} OpenGLVertexLayout;

typedef struct _gl_buffer {
	uint32_t id, type; // Buffer id
	uint32_t vertex_count, triangle_count;
	OpenGLVertexLayout layout;
} OpenGLBuffer;

static inline size_t attribute_format_to_bytes(AttributeFormat attribute_format);
static inline uint32_t attribute_format_to_count(AttributeFormat attribute_format);
static inline GLenum attribute_format_to_gl_type(AttributeFormat attribute_format);

void opengl_draw(struct _renderer *self, Buffer *vertex_buffer, uint32_t vertex_count) {
	if (vertex_buffer == NULL) {
		LOG_ERROR("Invalid buffer passed to draw function!");
		return;
	}
	OpenGLBuffer *gl_buffer = (OpenGLBuffer *)vertex_buffer;

	if (gl_buffer->layout.attributes == NULL) {
		LOG_ERROR("Can't draw buffer without layout!");
		return;
	}

	self->buffer_activate(self, vertex_buffer);
	const OpenGLVertexLayout *layout = &gl_buffer->layout;
	for (uint32_t i = 0; i < layout->count; i++) {
		const OpenGLVertexAttribute *attribute = &layout->attributes[i];
		GLenum type = attribute_format_to_gl_type(attribute->format);
		uint32_t count = attribute_format_to_count(attribute->format);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, count, type, GL_FALSE, layout->stride, (void *)(uintptr_t)attribute->offset);
	}

	glDrawArrays(GL_TRIANGLES, 0, vertex_count);

	for (uint32_t i = 0; i < layout->count; i++) {
		glDisableVertexAttribArray(i);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Buffer *opengl_buffer_create(struct _renderer *self, BufferType type, size_t size, void *data) {
	OpenGLBuffer *gl_buffer = malloc(sizeof(OpenGLBuffer));

	*gl_buffer = (OpenGLBuffer){ 0 };
	gl_buffer->type = type == BUFFER_TYPE_VERTEX ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;

	glGenBuffers(1, &gl_buffer->id);
	glBindBuffer(gl_buffer->type, gl_buffer->id);
	glBufferData(gl_buffer->type, size, data, GL_STATIC_DRAW);
	glBindBuffer(gl_buffer->type, 0);

	return gl_buffer;
}

void opengl_buffer_set_layout(struct _renderer *self, Buffer *buffer, VertexAttribute *attributes, uint32_t attribute_count) {
	if (attributes == NULL || buffer == NULL) {
		LOG_ERROR("Can't pass null arguments to buffer_set_layout!");
		return;
	}
	OpenGLBuffer *gl_buffer = (OpenGLBuffer *)buffer;

	gl_buffer->layout.attributes = malloc(sizeof(OpenGLVertexAttribute) * attribute_count);
	gl_buffer->layout.count = attribute_count;

	uint32_t offset = 0;
	for (uint32_t attribute_index = 0; attribute_index < attribute_count; attribute_index++) {
		VertexAttribute attribute = attributes[attribute_index];
		gl_buffer->layout.attributes[attribute_index] = (OpenGLVertexAttribute){
			.name = attribute.name,
			.offset = offset,
			.format = attribute.format,
			.location = attribute_index
		};
		offset += attribute_format_to_bytes(attribute.format);
	}
	gl_buffer->layout.stride = offset;
}

void opengl_buffer_destroy(struct _renderer *self, Buffer *buffer) {
	OpenGLBuffer *gl_buffer = (OpenGLBuffer *)buffer;

	if (buffer) {
		glDeleteBuffers(1, &gl_buffer->id);
		free(gl_buffer);
	}
}

void opengl_buffer_activate(struct _renderer *self, const Buffer *buffer) {
	OpenGLBuffer *gl_buffer = (OpenGLBuffer *)buffer;
	if (!buffer) {
		LOG_ERROR("Invalid buffer passed to buffer_activate function!");
		exit(1);
	}

	glBindBuffer(gl_buffer->type, gl_buffer->id);
}
void opengl_buffer_deactivate(struct _renderer *self, const Buffer *buffer) {
	OpenGLBuffer *gl_buffer = (OpenGLBuffer *)buffer;
	if (!buffer) {
		LOG_ERROR("Invalid buffer passed to buffer_deactivate function!");
		exit(1);
	}

	glBindBuffer(gl_buffer->type, 0);
}

size_t attribute_format_to_bytes(AttributeFormat attribute_format) {
	switch (attribute_format) {
		case FORMAT_FLOAT:
			return sizeof(float);
		case FORMAT_FLOAT2:
			return sizeof(float) * 2;
		case FORMAT_FLOAT3:
			return sizeof(float) * 3;
		case FORMAT_FLOAT4:
			return sizeof(float) * 4;
		default: {
			LOG_ERROR("Unkown attribute format type provided!");
			return 0;
		} break;
	}
}

uint32_t attribute_format_to_count(AttributeFormat attribute_format) {
	switch (attribute_format) {
		case FORMAT_FLOAT:
			return 1;
		case FORMAT_FLOAT2:
			return 2;
		case FORMAT_FLOAT3:
			return 3;
		case FORMAT_FLOAT4:
			return 4;
		default: {
			LOG_ERROR("Unkown attribute format type provided!");
			return 0;
		} break;
	}
}
GLenum attribute_format_to_gl_type(AttributeFormat attribute_format) {
	switch (attribute_format) {
		case FORMAT_FLOAT:
		case FORMAT_FLOAT2:
		case FORMAT_FLOAT3:
		case FORMAT_FLOAT4:
			return GL_FLOAT;
		default: {
			LOG_ERROR("Unkown attribute format type provided!");
			return 0;
		} break;
	}
}
