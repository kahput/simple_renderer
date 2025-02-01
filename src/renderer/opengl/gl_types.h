#pragma once
#include "renderer/gl_renderer.h"

typedef struct _gl_shader {
	uint32_t id; // Shader program id
	int32_t *locations; // TODO: Use this
} OpenGLShader;

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

typedef struct _gl_texture {
	uint32_t id;
	uint32_t width, height, channels;
	const char *path;
} OpenGLTexture;
