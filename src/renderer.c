#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

Renderer* renderer_create(RendererAPI backend, RendererCreateInfo* create_info) {
	switch (backend) {
		case BACKEND_API_NONE: {
			printf("ERROR: None renderer not supported!\n");
			exit(1);
		};
		case BACKEND_API_OPENGL: {
			return renderer_opengl_create();
		} break;
		case BACKEND_API_VULKAN: {
			return renderer_vulkan_create(create_info);
		} break;
		default: {
			printf("ERROR: Unknown renderer!\n");
			exit(1);
		} break;
	};
}
void renderer_destroy(Renderer* renderer) {
	if (renderer) {
		switch (renderer->type) {
			case BACKEND_API_NONE: {
				printf("ERROR: None renderer not supported!\n");
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
				printf("ERROR: Unknown renderer!\n");
				exit(1);
			} break;
		};
	}
}
