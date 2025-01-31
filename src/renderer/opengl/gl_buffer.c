#include "base.h"
#include "renderer.h"
#include "renderer/gl_renderer.h"

#include <glad/gl.h>
#include <stdlib.h>

typedef struct _gl_buffer {
	uint32_t id; // Buffer id
	uint32_t vertex_count, triangle_count;
	uint32_t type;
} OpenGLBuffer;

Buffer* opengl_buffer_create(struct _renderer* self, BufferType type, size_t size, void* data) {
	OpenGLBuffer* gl_buffer = malloc(sizeof(OpenGLBuffer));

	gl_buffer->type = type == BUFFER_TYPE_VERTEX ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;

	glGenBuffers(1, &gl_buffer->id);
	glBindBuffer(gl_buffer->type, gl_buffer->id);
	glBufferData(gl_buffer->type, size, data, GL_STATIC_DRAW);
	glBindBuffer(gl_buffer->type, 0);

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

	glBindBuffer(gl_buffer->type, gl_buffer->id);
}
void opengl_buffer_deactivate(struct _renderer* self, const Buffer* buffer) {
	OpenGLBuffer* gl_buffer = (OpenGLBuffer*)buffer;
	if (!buffer) {
		LOG_ERROR("Invalid buffer passed to buffer_deactivate function!");
		exit(1);
	}

	glBindBuffer(gl_buffer->type, 0);
}
