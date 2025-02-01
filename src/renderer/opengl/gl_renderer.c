#include "renderer/gl_renderer.h"
#include "renderer.h"

#include <glad/gl.h>
#include <stdlib.h>

typedef struct _gl_renderer {
	Renderer base;
	uint32_t vao;
} OpenGLRenderer;

Renderer *opengl_renderer_create() {
	OpenGLRenderer *renderer = malloc(sizeof(OpenGLRenderer));
	renderer->base.backend = BACKEND_API_OPENGL;

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	// Drwa
	renderer->base.draw = opengl_draw;

	// Buffer -----------------------------------------------------
	renderer->base.buffer_create = opengl_buffer_create;
	renderer->base.buffer_destroy = opengl_buffer_destroy;
	renderer->base.buffer_set_layout = opengl_buffer_set_layout;
	renderer->base.buffer_activate = opengl_buffer_activate;
	renderer->base.buffer_deactivate = opengl_buffer_deactivate;

	// Shader -----------------------------------------------------
	renderer->base.texture_load = opengl_texture_load;
	renderer->base.texture_destroy = opengl_texture_destroy;
	renderer->base.texture_activate = opengl_texture_activate;

	// Shader -----------------------------------------------------
	renderer->base.shader_from_file = opengl_shader_from_file;
	renderer->base.shader_from_string = opengl_shader_from_string;
	renderer->base.shader_destroy = opengl_shader_destroy;

	renderer->base.shader_activate = opengl_shader_activate;
	renderer->base.shader_deactivate = opengl_shader_deactivate;

	renderer->base.shader_seti = opengl_shader_seti;
	renderer->base.shader_setf = opengl_shader_setf;
	renderer->base.shader_set2fv = opengl_shader_set2fv;
	renderer->base.shader_set3fv = opengl_shader_set3fv;
	renderer->base.shader_set4fv = opengl_shader_set4fv;
	renderer->base.shader_set4fm = opengl_shader_set4fm;

	return &renderer->base;
}

void opengl_renderer_destroy(Renderer *renderer) {
	return;
}
