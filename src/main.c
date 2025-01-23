#include "defines.h"
#include "renderer.h"
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
		"uniform vec4 u_color;\n"
		"void main()\n"
		"{\n"
		"   fragment_color = vec4(u_color);\n"
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

	Shader* shader = renderer_shader_str(vertex_shader_source, fragment_shader_source, NULL);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glfwSwapBuffers(window);

		glClearColor(0.95f, .95f, .95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = glfwGetTime();
		vec4 u_color = { (cos(time) / 2.f) + .5f, (sin(time) / 2.f) + .5f, 0.0f, 1.0f };

		glBindVertexArray(vertex_array_object);
		renderer_shader_activate(shader);
		renderer_shader_set4fv(shader, "u_color", u_color);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	renderer_shader_destroy(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
