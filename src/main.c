#include "defines.h"
#include <stdint.h>
#include <stdio.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

const char* vertex_shader_source =
		"#version 450 core\n"
		"layout (location = 0) in vec3 attrib_position;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(attrib_position, 1.0);\n"
		"}\0";

const char* fragment_shader_source =
		"#version 450 core\n"
		"out vec4 fragment_color;\n"
		"void main()\n"
		"{\n"
		"   fragment_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
int main(void) {
	/**
	 * ===========================================================================================
	 * -------- Window creation
	 * ===========================================================================================
	 **/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Noise", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);

	/**
	 * ===========================================================================================
	 * -------- Vertex array object creation
	 * ===========================================================================================
	 **/

	// clang-format off
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	uint32_t indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// clang-format on

	// Create vertex array object
	uint32_t vertex_array_object = 0;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	// Create Vertex buffer object
	uint32_t vertex_buffer_object = 0;
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof *vertices), (void*)NULL);
	glEnableVertexAttribArray(0);

	uint32_t index_buffer_object = 0;
	glGenBuffers(1, &index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/**
	 * ===========================================================================================
	 * -------- Shader creation
	 * ===========================================================================================
	 **/

	uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	int32_t success;
	char info_buffer[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_buffer);
		printf("ERROR:SHADER:VERTEX:COMPILATION_FAILED | %s\n", info_buffer);
	}

	uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_buffer);
		printf("ERROR:SHADER:FRAGMENT:COMPILATION_FAILED | %s\n", info_buffer);
	}

	uint32_t program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_buffer);
		printf("ERROR:SHADER:LINKING_FAILED | %s\n", info_buffer);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);

		glClearColor(0.95f, .95f, .95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(vertex_array_object);
		glUseProgram(program);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
