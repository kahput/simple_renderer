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
	BUFFER_TYPE_VERTEX,
	BUFFER_TYPE_INDEX,

	BUFFER_TYPE_COUNT
} BufferType;

typedef enum {
	FORMAT_FLOAT,
	FORMAT_FLOAT2,
	FORMAT_FLOAT3,
	FORMAT_FLOAT4,

	FORMAT_COUNT
} AttributeFormat;

typedef struct {
	const char *name;
	AttributeFormat format;
} VertexAttribute;

typedef enum {
	PROJECTION_PERSPECTIVE,
	PROJECTION_ORTHOGRAPHIC,
	PROJECTION_FRUSTUM
} ProjectionType;

typedef void Shader;
typedef void Buffer;
typedef void Texture;
typedef struct _camera Camera;

/*
 * ===========================================================================================
 * -------- Shader
 * ===========================================================================================
 **/

typedef struct _renderer {
	void (*frame_begin)(struct _renderer *self);
	void (*frame_end)(struct _renderer *self);

	void (*draw)(struct _renderer *self, Buffer *vertex_buffer, uint32_t vertex_count);

	// Buffers
	Buffer *(*buffer_create)(struct _renderer *self, BufferType type, size_t size, void *data);
	void (*buffer_set_layout)(struct _renderer *self, Buffer *buffer, VertexAttribute *attributes, uint32_t attribute_count);
	void (*buffer_destroy)(struct _renderer *self, Buffer *buffer);

	void (*buffer_activate)(struct _renderer *self, const Buffer *buffer);
	void (*buffer_deactivate)(struct _renderer *self, const Buffer *buffer);

	// Textures
	Texture *(*texture_load)(struct _renderer *self, const char *texture_path);
	void (*texture_destroy)(struct _renderer *self, Texture *texture);

	void (*texture_activate)(struct _renderer *self, Texture *texture, uint32_t texture_unit);

	// Shaders
	Shader *(*shader_from_file)(const char *vertex_shader_path, const char *fragment_shader_path, const char *geometry_shader_source);
	Shader *(*shader_from_string)(const char *vertex_shader_source, const char *fragment_shader_source, const char *geometry_shader_source);
	void (*shader_destroy)(Shader *shader);

	void (*shader_activate)(Shader *shader);
	void (*shader_deactivate)(Shader *shader);

	void (*shader_seti)(Shader *shader, const char *name, int32_t value);
	void (*shader_setf)(Shader *shader, const char *name, float value);
	void (*shader_set2fv)(Shader *shader, const char *name, float *value);
	void (*shader_set3fv)(Shader *shader, const char *name, float *value);
	void (*shader_set4fv)(Shader *shader, const char *name, float *value);
	void (*shader_set4fm)(Shader *shader, const char *name, float *value);

	RendererAPI backend;
} Renderer;

Renderer *renderer_create(RendererAPI backend);
void renderer_destroy(Renderer *renderer);

/**
 * ===========================================================================================
 * -------- Camera
 * ===========================================================================================
 **/
// TODO: Find better place for camera

Camera *camera_create();
void camera_destroy(Camera *camera);

void camera_set_perspective(Camera *camera, float fov, float near, float far);
void camera_set_orthogonal(Camera *camera, float size, float near, float far);

void camera_update(Camera *camera, float camera_position[3], float camera_front[3], float camera_up[3]);

float *camera_get_view(Camera *camera);
float *camera_get_projection(Camera *camera);
