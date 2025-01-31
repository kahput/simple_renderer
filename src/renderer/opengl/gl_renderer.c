#include "renderer/gl_renderer.h"
#include "renderer.h"

#include <stdlib.h>

typedef struct _ogl_renderer {
	Renderer base;
} OpenGLRenderer;

Renderer* opengl_renderer_create() {
	OpenGLRenderer* renderer = malloc(sizeof(OpenGLRenderer));
	renderer->base.backend = BACKEND_API_OPENGL;

	// Buffer -----------------------------------------------------
	renderer->base.buffer_create = opengl_buffer_create;
	renderer->base.buffer_destroy = opengl_buffer_destroy;
	renderer->base.buffer_activate = opengl_buffer_activate;
	renderer->base.buffer_deactivate = opengl_buffer_deactivate;

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

void opengl_renderer_destroy(Renderer* renderer) {
	return;
}
