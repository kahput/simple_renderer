#include "renderer.h"
#include "base.h"
#include "renderer/gl_renderer.h"
#include <stdlib.h>

Renderer* renderer_create(RendererAPI backend) {
	const char* backend_stringify[] = {
		"None",
		"OpenGL",
		"Vulkan",
	};

	switch (backend) {
		case BACKEND_API_OPENGL: {
			return opengl_renderer_create();
		} break;
		case BACKEND_API_NONE:
		case BACKEND_API_VULKAN:
		default: {
			LOG_ERROR("Renderer %s is not supported at this moment!", backend_stringify[backend]);
			exit(1);
		} break;
	};
}
void renderer_destroy(Renderer* renderer) {
	const char* backend_stringify[] = {
		"None",
		"OpenGL",
		"Vulkan",
	};

	if (renderer) {
		switch (renderer->backend) {
			case BACKEND_API_OPENGL: {
				opengl_renderer_destroy(renderer);
				free(renderer);
			} break;
			case BACKEND_API_NONE:
			case BACKEND_API_VULKAN:
			default: {
				LOG_ERROR("Renderer %s is not supported at this moment!", backend_stringify[renderer->backend]);
				exit(1);
			} break;
		};
	}
}
