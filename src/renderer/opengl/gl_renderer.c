#include "renderer.h"
#include "renderer/gl_renderer.h"

#include <stdlib.h>

typedef struct _ogl_renderer {
	Renderer base;
} OpenGLRenderer;

Renderer* renderer_opengl_create() {
	OpenGLRenderer* renderer = malloc(sizeof(OpenGLRenderer));
	renderer->base.type = BACKEND_API_OPENGL;

	*renderer = (OpenGLRenderer){
		.base = {
			.type = BACKEND_API_OPENGL,
			.shader_from_file = opengl_shader_from_file,
			.shader_from_string = opengl_shader_from_string,
			.shader_destroy = opengl_shader_destroy,
			.shader_activate = opengl_shader_activate,
			.shader_deactivate = opengl_shader_deactivate,
			.shader_seti = opengl_shader_seti,
			.shader_setf = opengl_shader_setf,
			.shader_set2fv = opengl_shader_set2fv,
			.shader_set3fv = opengl_shader_set3fv,
			.shader_set4fv = opengl_shader_set4fv,
			.shader_set4fm = opengl_shader_set4fm

		},
	};

	return &renderer->base;
}

void renderer_opengl_destroy(Renderer* renderer) {
	return;
}
