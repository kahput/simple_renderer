#include "base.h"
#include "renderer/gl_renderer.h"

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _gl_buffer {
	uint32_t id; // Buffer id
	uint32_t vertex_count, triangle_count;
	BufferUsage usage;
} OpenGLBuffer;

Buffer* opengl_buffer_create(struct _renderer* self, const BufferCreateInfo* info) {
	OpenGLBuffer* gl_buffer = malloc(sizeof(OpenGLBuffer));

	switch (info->usage) {
		case BUFFER_USAGE_VERTEX: {
			glGenBuffers(1, &gl_buffer->id);
			glBindBuffer(GL_ARRAY_BUFFER, gl_buffer->id);
			glBufferData(GL_ARRAY_BUFFER, info->size, info->data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} break;
		case BUFFER_USAGE_INDEX: {
			glGenBuffers(1, &gl_buffer->id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer->id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, info->size, info->data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		} break;
		default:
			break;
	}

	return gl_buffer;
}
void opengl_buffer_destroy(struct _renderer* self, Buffer* buffer) {
	OpenGLBuffer* gl_buffer = (OpenGLBuffer*)buffer;

	if (buffer) {
		glDeleteBuffers(1, &gl_buffer->id);
		free(gl_buffer);
	}
}

void opengl_buffer_activate(struct _renderer* self, const Buffer* buffer) {
	OpenGLBuffer* gl_buffer = (OpenGLBuffer*)buffer;
	if (!buffer) {
		LOG_ERROR("Invalid buffer passed to buffer_activate function!");
		exit(1);
	}
	switch (gl_buffer->usage) {
		case BUFFER_USAGE_VERTEX: {
			glBindBuffer(GL_ARRAY_BUFFER, gl_buffer->id);
		} break;
		case BUFFER_USAGE_INDEX: {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_buffer->id);
		} break;
		default:
			break;
	}
}
void opengl_buffer_deactivate(struct _renderer* self, const Buffer* buffer) {
	OpenGLBuffer* gl_buffer = (OpenGLBuffer*)buffer;
	if (!buffer) {
		LOG_ERROR("Invalid buffer passed to buffer_deactivate function!");
		exit(1);
	}
	switch (gl_buffer->usage) {
		case BUFFER_USAGE_VERTEX: {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} break;
		case BUFFER_USAGE_INDEX: {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		} break;
		default:
			break;
	}
}
