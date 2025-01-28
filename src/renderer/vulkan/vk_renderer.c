#include "renderer/vk_renderer.h"
#include "base.h"
#include "renderer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _vk_renderer {
	Renderer base;
	VkInstance instance;
	VkDebugUtilsMessengerEXT logger;
	bool enable_validation_layer;
} VulkanRenderer;

static const char* validation_layers[] = {
	"VK_LAYER_KHRONOS_validation"

};

void vk_create_instance(VulkanRenderer* renderer, const char** (*get_extensions)(uint32_t* count));
void setup_logger(VulkanRenderer* renderer);

Renderer* renderer_vulkan_create(RendererCreateInfo* create_info) {
	VulkanRenderer* renderer = malloc(sizeof(VulkanRenderer));

#ifdef NDEBUG
	renderer->enable_validation_layer = false;
#else
	renderer->enable_validation_layer = true;
#endif

	if (!create_info || !create_info->extension_info) {
		LOG_ERROR("RendererCreateInfo.extension_info needed to create VulkanRenderer");
		exit(1);
	}
	vk_create_instance(renderer, create_info->extension_info);
	setup_logger(renderer);
	*renderer = (VulkanRenderer){
		.base = {
			.type = BACKEND_API_VULKAN

		},
	};

	return &renderer->base;
}

void renderer_vulkan_destroy(Renderer* renderer) {
	VulkanRenderer* vk_renderer = (VulkanRenderer*)renderer;
	if (vk_renderer->enable_validation_layer)
		vkDestroyDebugUtilsMessengerEXT(vk_renderer->instance, vk_renderer->logger, NULL);
	vkDestroyInstance(vk_renderer->instance, NULL);
}

void vk_create_instance(VulkanRenderer* renderer, const char** (*get_extensions)(uint32_t* count)) {
	if (renderer->enable_validation_layer && !vulkan_validation_layer_support()) {
		LOG_ERROR("Validation layers requested, but not available!");
		exit(1);
	}

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

	uint32_t platform_extension_count = 0;
	const char** platform_extensions;
	platform_extensions = get_extensions(&platform_extension_count);

	create_info.enabledExtensionCount = platform_extension_count;
	create_info.ppEnabledExtensionNames = platform_extensions;

	create_info.enabledLayerCount = 0;
	if (renderer->enable_validation_layer) {
		LOG_INFO("Validation layer enabled");
		create_info.enabledLayerCount = sizeof(validation_layers) / sizeof(*validation_layers);
		create_info.ppEnabledLayerNames = validation_layers;

		const char** extensions = malloc(sizeof(const char*) * create_info.enabledExtensionCount + 1);
		for (int i = 0; i < platform_extension_count; i++) {
			extensions[i] = platform_extensions[i];
		}

		extensions[platform_extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		create_info.enabledExtensionCount += 1;
		create_info.ppEnabledExtensionNames = extensions;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info = { 0 };
		vulkan_populate_messenger_create_info(&debug_create_info);
		create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_create_info;
	}

	platform_extension_count = 0;
	vkEnumerateInstanceExtensionProperties(NULL, &platform_extension_count, NULL);
	VkExtensionProperties available_extensions[platform_extension_count];
	vkEnumerateInstanceExtensionProperties(NULL, &platform_extension_count, available_extensions);

	// LOG_TRACE("- Required extensions:");
	// for (int i = 0; i < create_info.enabledExtensionCount; i++)
	// 	LOG_TRACE("\t* %s", create_info.ppEnabledExtensionNames[i]);
	//
	// LOG_TRACE("- Available extensions:");
	// for (int i = 0; i < platform_extension_count; i++)
	// 	LOG_TRACE("\t* %s", available_extensions[i].extensionName);

	if (vkCreateInstance(&create_info, NULL, &renderer->instance)) {
		LOG_ERROR("Failed to create vulkan instance!");
		exit(1);
	}

	vulkan_validation_layer_support();
}

void setup_logger(VulkanRenderer* renderer) {
	if (!renderer->enable_validation_layer)
		return;

	VkDebugUtilsMessengerCreateInfoEXT create_info = { 0 };
	vulkan_populate_messenger_create_info(&create_info);

	if (vkCreateDebugUtilsMessengerEXT(renderer->instance, &create_info, NULL, &renderer->logger) != VK_SUCCESS) {
		LOG_ERROR("Failed to setup debug logger!");
		exit(1);
	}
}

bool vulkan_validation_layer_support() {
	uint32_t layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, NULL);

	VkLayerProperties available_layers[layer_count];
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

	for (uint32_t i = 0; i < (sizeof(validation_layers) / sizeof(*validation_layers)); i++) {
		bool layer_found = false;

		for (uint32_t j = 0; j < layer_count; j++) {
			if (strcmp(validation_layers[i], available_layers[j].layerName) == 0) {
				layer_found = true;
			}
		}
		return layer_found;
	}
	return true;
}

void vulkan_populate_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT* create_info) {
	create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	create_info->pfnUserCallback = vk_debug_callback;
	create_info->pUserData = NULL;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data) {
	switch (message_severity) {
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
			LOG_DEBUG(callback_data->pMessage);
		} break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
			LOG_TRACE(callback_data->pMessage);
		} break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
			LOG_WARN(callback_data->pMessage);
		} break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
			LOG_ERROR(callback_data->pMessage);
		} break;
		default:
			break;
	}

	return false;
}

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pMessenger) {
	PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func) {
		return func(instance, pCreateInfo, pAllocator, pMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) {
	PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func)
		func(instance, messenger, pAllocator);
}
