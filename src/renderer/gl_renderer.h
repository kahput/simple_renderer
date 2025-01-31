#pragma once
#include "renderer.h"

#include <stdint.h>

Renderer *opengl_renderer_create();
void opengl_renderer_destroy(Renderer *renderer);

void opengl_draw(struct _renderer *self, Buffer *vertex_buffer, uint32_t vertex_count);

/*
 * ===========================================================================================
 * -------- Buffer
 * ===========================================================================================
 **/

Buffer *opengl_buffer_create(struct _renderer *self, BufferType type, size_t size, void *data);
void opengl_buffer_set_layout(struct _renderer *self, Buffer *buffer, VertexAttribute *attributes, uint32_t attribute_count);
void opengl_buffer_destroy(struct _renderer *self, Buffer *buffer);

void opengl_buffer_activate(struct _renderer *self, const Buffer *buffer);
void opengl_buffer_deactivate(struct _renderer *self, const Buffer *buffer);

/*
 * ===========================================================================================
 * -------- Shader
 * ===========================================================================================
 **/

Shader *opengl_shader_from_file(const char *vertex_shader_path, const char *fragment_shader_path, const char *geometry_shader_source);
Shader *opengl_shader_from_string(const char *vertex_shader_source, const char *fragment_shader_source, const char *geometry_shader_source);
void opengl_shader_destroy(Shader *shader);

void opengl_shader_activate(Shader *shader);
void opengl_shader_deactivate(Shader *shader);

void opengl_shader_seti(Shader *shader, const char *name, int32_t value);
void opengl_shader_setf(Shader *shader, const char *name, float value);
void opengl_shader_set2fv(Shader *shader, const char *name, float *value);
void opengl_shader_set3fv(Shader *shader, const char *name, float *value);
void opengl_shader_set4fv(Shader *shader, const char *name, float *value);
void opengl_shader_set4fm(Shader *shader, const char *name, float *value);
