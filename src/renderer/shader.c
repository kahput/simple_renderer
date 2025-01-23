#include "renderer.h"

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _shader {
	uint32_t id; // Shader program id
	int32_t* locations; // TODO: Use this
};

Shader* renderer_shader_file(const char* vertex_shader_path, const char* fragment_shader_path, const char* geometry_shader_path) {
	return NULL;
}
Shader* renderer_shader_str(const char* vertex_shader_source, const char* fragment_shader_source, const char* geometry_shader_source) {
	Shader* shader = malloc(sizeof(struct _shader));
	uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	int32_t success;
	char info_buffer[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_buffer);
		printf("ERROR:SHADER:VERTEX:COMPILATION_FAILED | %s\n", info_buffer);
		return NULL;
	}

	uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_buffer);
		printf("ERROR:SHADER:FRAGMENT:COMPILATION_FAILED | %s\n", info_buffer);
		return NULL;
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_buffer);
		printf("ERROR:SHADER:LINKING_FAILED | %s\n", info_buffer);
		return NULL;
	}

	shader->id = program;
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader;
}
void renderer_shader_destroy(Shader* shader) {
	glDeleteProgram(shader->id);
	free(shader);
}

void renderer_shader_activate(Shader* shader) {
	glUseProgram(shader->id);
}
void renderer_shader_deactivate(Shader* shader) {
	glUseProgram(0);
}

void renderer_shader_seti(Shader* shader, const char* name, int32_t value) {}
void renderer_shader_setf(Shader* shader, const char* name, float value) {
	glUniform1f(glGetUniformLocation(shader->id, name), value);
}
void renderer_shader_set2fv(Shader* shader, const char* name, float* value) {
	glUniform2fv(glGetUniformLocation(shader->id, name), 1, value);
}
void renderer_shader_set3fv(Shader* shader, const char* name, float* value) {
	glUniform3fv(glGetUniformLocation(shader->id, name), 1, value);
}
void renderer_shader_set4fv(Shader* shader, const char* name, float* value) {
	glUniform4fv(glGetUniformLocation(shader->id, name), 1, value);
}
void renderer_shader_set4fm(Shader* shader, const char* name, float* value) {
}
