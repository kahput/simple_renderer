#include "renderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

typedef struct _vk_renderer {
	Renderer base;
	VkInstance instance;
} VulkanRenderer;

void vk_create_instance(VulkanRenderer* renderer, const char** (*get_extensions)(uint32_t* count));

Renderer* renderer_vulkan_create(RendererCreateInfo* create_info) {
	VulkanRenderer* renderer = malloc(sizeof(VulkanRenderer));
	if (!create_info || !create_info->extension_info) {
		printf("ERROR: RendererCreateInfo.extension_info needed to create VulkanRenderer\n");
		exit(1);
	}
	vk_create_instance(renderer, create_info->extension_info);
	*renderer = (VulkanRenderer){
		.base = {

		},
	};

	return &renderer->base;
}

void renderer_vulkan_destroy(Renderer* renderer) {
	VulkanRenderer* vk_renderer = (VulkanRenderer*)renderer;
	vkDestroyInstance(vk_renderer->instance, NULL);
}

void vk_create_instance(VulkanRenderer* renderer, const char** (*get_extensions)(uint32_t* count)) {
	VkApplicationInfo app_info = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Simple Craft",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Simple renderer",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3
	};

	VkInstanceCreateInfo create_info = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &app_info
	};

	uint32_t extension_count = 0;
	const char** extensions;

	extensions = get_extensions(&extension_count);
	create_info.enabledExtensionCount = extension_count;
	create_info.ppEnabledExtensionNames = extensions;

	create_info.enabledLayerCount = 0;

	if (vkCreateInstance(&create_info, NULL, &renderer->instance)) {
		printf("ERROR: Failed to create vulkan instance!\n");
		exit(1);
	}
}
