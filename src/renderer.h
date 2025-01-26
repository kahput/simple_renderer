#pragma once

#include <stdint.h>

typedef enum {
	PROJECTION_PERSPECTIVE,
	PROJECTION_ORTHOGRAPHIC,
	PROJECTION_FRUSTUM
} ProjectionType;

typedef struct _shader Shader;
typedef struct _camera Camera;

/**
 * ===========================================================================================
 * -------- Shader
 * ===========================================================================================
 **/

Shader* renderer_shader_from_file(const char* vertex_shader_path, const char* fragment_shader_path, const char* geometry_shader_source);
Shader* renderer_shader_from_string(const char* vertex_shader_source, const char* fragment_shader_source, const char* geometry_shader_source);
void renderer_shader_destroy(Shader* shader);

void renderer_shader_activate(Shader* shader);
void renderer_shader_deactivate(Shader* shader);

void renderer_shader_seti(Shader* shader, const char* name, int32_t value);
void renderer_shader_setf(Shader* shader, const char* name, float value);
void renderer_shader_set2fv(Shader* shader, const char* name, float* value);
void renderer_shader_set3fv(Shader* shader, const char* name, float* value);
void renderer_shader_set4fv(Shader* shader, const char* name, float* value);
void renderer_shader_set4fm(Shader* shader, const char* name, float* value);

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

float* camera_fetch_view(Camera* camera);
float* camera_fetch_projection(Camera* camera);

void renderer_frame_resize(void* platfom, int32_t width, int32_t height);

void renderer_begin_frame(Camera* camera);
void renderer_end_frame();
