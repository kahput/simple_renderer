#include "renderer.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>

Renderer* renderer_create(RendererAPI backend, RendererCreateInfo* create_info) {
	switch (backend) {
		case BACKEND_API_NONE: {
			LOG_ERROR("None renderer not supported!");
			exit(1);
		};
		case BACKEND_API_OPENGL: {
			return renderer_opengl_create();
		} break;
		case BACKEND_API_VULKAN: {
			return renderer_vulkan_create(create_info);
		} break;
		default: {
			LOG_ERROR("Unknown renderer!");
			exit(1);
		} break;
	};
}
void renderer_destroy(Renderer* renderer) {
	if (renderer) {
		switch (renderer->type) {
			case BACKEND_API_NONE: {
				LOG_ERROR("None renderer not supported!");
				exit(1);
			};
			case BACKEND_API_OPENGL: {
				renderer_opengl_destroy(renderer);
				free(renderer);
			} break;
			case BACKEND_API_VULKAN: {
				renderer_vulkan_destroy(renderer);
				free(renderer);
			} break;
			default: {
				LOG_ERROR("Unknown renderer!");
				exit(1);
			} break;
		};
	}
}
