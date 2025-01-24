#pragma once

#include <stdint.h>

#define renderer_shader_set(name, value)

typedef struct _shader Shader;

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
void renderer_shader_set4fm(Shader* shader, const char* name, float value[4][4]);
