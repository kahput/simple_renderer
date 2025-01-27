#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum {
	BACKEND_API_NONE,
	BACKEND_API_OPENGL,
	BACKEND_API_VULKAN,

	BACKEND_COUNT
} RendererAPI;

typedef enum {
	BUFFER_USAGE_VERTEX,
	BUFFER_USAGE_INDEX,

	BUFFER_USAGE_COUNT
} BufferUsage;

typedef enum {
	PROJECTION_PERSPECTIVE,
	PROJECTION_ORTHOGRAPHIC,
	PROJECTION_FRUSTUM
} ProjectionType;

typedef void Shader;
typedef void Buffer;
typedef struct _camera Camera;

typedef struct _renderer_create_info {
	const char** (*extension_info)(uint32_t* count);
} RendererCreateInfo;

typedef struct _buffer_create_info {
	BufferUsage usage;
	size_t size;
	void* data;
} BufferCreateInfo;

/*
 * ===========================================================================================
 * -------- Shader
 * ===========================================================================================
 **/

typedef struct _renderer {
	void (*frame_begin)(struct _renderer* self);
	void (*frame_end)(struct _renderer* self);

	// Buffers
	Buffer *(*buffer_create)(struct _renderer* self, const BufferCreateInfo* buffer);
	void (*buffer_destroy)(struct _renderer* self, Buffer* buffer);

	void (*buffer_activate)(struct _renderer* self, const Buffer* buffer);
	void (*buffer_deactivate)(struct _renderer* self, const Buffer* buffer);

	// Shaders
	Shader* (*shader_from_file)(const char* vertex_shader_path, const char* fragment_shader_path, const char* geometry_shader_source);
	Shader* (*shader_from_string)(const char* vertex_shader_source, const char* fragment_shader_source, const char* geometry_shader_source);
	void (*shader_destroy)(Shader* shader);

	void (*shader_activate)(Shader* shader);
	void (*shader_deactivate)(Shader* shader);

	void (*shader_seti)(Shader* shader, const char* name, int32_t value);
	void (*shader_setf)(Shader* shader, const char* name, float value);
	void (*shader_set2fv)(Shader* shader, const char* name, float* value);
	void (*shader_set3fv)(Shader* shader, const char* name, float* value);
	void (*shader_set4fv)(Shader* shader, const char* name, float* value);
	void (*shader_set4fm)(Shader* shader, const char* name, float* value);

	RendererAPI type;
} Renderer;

Renderer* renderer_create(RendererAPI backend, RendererCreateInfo* create_info);
Renderer* renderer_opengl_create();
Renderer* renderer_vulkan_create(RendererCreateInfo* create_info);

void renderer_destroy(Renderer* renderer);
void renderer_opengl_destroy(Renderer* renderer);
void renderer_vulkan_destroy(Renderer* renderer);

/**
 * ===========================================================================================
 * -------- Camera
 * ===========================================================================================
 **/
// TODO: Find better place for camera

Camera* camera_create();
void camera_destroy(Camera* camera);

void camera_set_perspective(Camera* camera, float fov, float near, float far);
void camera_set_orthogonal(Camera* camera, float size, float near, float far);

void camera_update(Camera* camera, float camera_position[3], float camera_front[3], float camera_up[3]);

float* camera_get_view(Camera* camera);
float* camera_get_projection(Camera* camera);
