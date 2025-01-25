#include "defines.h"
#include "renderer.h"
#include <cglm/affine-pre.h>
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/util.h>
#include <cglm/vec3.h>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/affine-mat.h>
#include <cglm/cglm.h>
#include <stb/stb_image.h>

#include <stdint.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define Min(a, b) (((a) < (b)) ? a : b)
#define Max(a, b) (((a) > (b)) ? a : b)

void get_mouse_offset(GLFWwindow* window, float* x_offset, float* y_offset);
void mouse_callback(GLFWwindow* window, double x, double y);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	/**
	 * ===========================================================================================
	 * -------- Vertex array object creation
	 * ===========================================================================================
	 **/

	// clang-format off
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// uint32_t indices[] = {
	// 	0, 1, 3,   // first triangle
	// 	1, 2, 3    // second triangle
	// };
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)(3 * (sizeof *vertices)));
	glEnableVertexAttribArray(1);

	// uint32_t index_buffer_object = 0;
	// glGenBuffers(1, &index_buffer_object);
	//
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/**
	 * ===========================================================================================
	 * -------- Texture creation
	 * ===========================================================================================
	 **/

	uint32_t texture[2];
	glGenTextures(2, texture);

	// First texture
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int32_t width, height, channel_count;
	const char* paths[] = { "assets/textures/container.jpg", "assets/textures/awesomeface.png" };
	uint8_t* data = stbi_load(paths[0], &width, &height, &channel_count, 0);
	if (!data) {
		printf("ERROR:TEXTURE:FILE [ %s ] NOT_FOUND\n", paths[0]);
		return 1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Second texture
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	data = stbi_load(paths[1], &width, &height, &channel_count, 0);
	if (!data) {
		printf("ERROR:TEXTURE:FILE [ %s ] NOT_FOUND\n", paths[1]);
		return 1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	/**
	 * ===========================================================================================
	 * -------- Shader creation
	 * ===========================================================================================
	 **/
	Shader* shader = renderer_shader_from_file("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl", NULL);
	renderer_shader_activate(shader);
	renderer_shader_seti(shader, "u_texture_1", 0);
	renderer_shader_seti(shader, "u_texture_2", 1);

	/**
	 * ===========================================================================================
	 * -------- Math
	 * ===========================================================================================
	 **/

	vec3 positions[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f },
		{ -3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{ -1.7f, 3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f, 2.0f, -2.5f },
		{ 1.5f, 0.2f, -1.5f },
		{ -1.3f, 1.0f, -1.5f }
	};

	mat4 model, view, projection;

	glm_mat4_identity(model);
	glm_rotate(model, glm_rad(-55.0f), (vec3){ 1.0f, 0.0f, 0.0f });

	glm_mat4_identity(view);
	glm_translate(view, (vec3){ 0.0f, 0.0f, -5.0f });

	glm_mat4_identity(projection);
	glm_perspective(glm_rad(45.f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

	/**
	 * ===========================================================================================
	 * -------- Camera
	 * ===========================================================================================
	 **/

	vec3 camera_position = { 0.f, 0.f, 5.0f }, camera_up = { 0.0f, 1.0f, 0.0f }, camera_front = { 0.0f, 0.0f, -1.0f }, camera_right = { 1.0f, 0.0f, 0.0f };
	float yaw = -90.0f, pitch = 0.0f;
	float camera_speed = 5.0f;

	float delta_time = 0.0f;
	float last_frame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		float x_offset = 0.0f, y_offset = 0.0f;
		get_mouse_offset(window, &x_offset, &y_offset);

		const float sensitivity = 2.f;

		yaw += x_offset * delta_time * 2.f;
		pitch += y_offset * delta_time * 2.f;
		pitch = Max(-89.0f, Min(89.0f, pitch));

		glfwPollEvents();
		glfwSwapBuffers(window);

		vec3 camera_movement = {
			(glfwGetKey(window, GLFW_KEY_D) % 2) - (glfwGetKey(window, GLFW_KEY_A) % 2),
			0.0f,
			(glfwGetKey(window, GLFW_KEY_W) % 2) - (glfwGetKey(window, GLFW_KEY_S) % 2)
		};

		glm_mat4_identity(view);

		camera_front[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
		camera_front[1] = sin(glm_rad(pitch));
		camera_front[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

		glm_cross(camera_front, camera_up, camera_right);
		glm_normalize(camera_right);

		glm_vec3_muladds(camera_right, camera_movement[0] * delta_time * camera_speed, camera_position);
		glm_vec3_muladds(camera_front, camera_movement[2] * delta_time * camera_speed, camera_position);

		glm_look(camera_position, camera_front, camera_up, view);

		glClearColor(0.95f, .95f, .95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();
		vec4 u_color = { (cos(time) / 2.f) + .5f, (sin(time) / 2.f) + .5f, 0.0f, 1.0f };

		renderer_shader_set4fv(shader, "u_color", u_color);
		renderer_shader_set4fm(shader, "u_view", view);
		renderer_shader_set4fm(shader, "u_projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glBindVertexArray(vertex_array_object);
		for (int i = 0; i < (sizeof(positions) / sizeof *positions); i++) {
			glm_mat4_identity(model);
			glm_translate(model, positions[i]);
			float angle = 20 * i;
			glm_rotate(model, glm_rad(angle), (vec3){ 1.0f, 0.3f, 0.5f });
			renderer_shader_set4fm(shader, "u_model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
	}

	renderer_shader_destroy(shader);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void get_mouse_offset(GLFWwindow* window, float* x_offset, float* y_offset) {
	static bool first_frame = true;
	static double last_position_x = 0.0f, last_position_y = 0.0f;

	double current_position_x, current_position_y;
	glfwGetCursorPos(window, &current_position_x, &current_position_y);

	if (first_frame) {
		last_position_x = current_position_x;
		last_position_y = current_position_y;
		first_frame = false;
		return;
	}

	*x_offset = current_position_x - last_position_x;
	*y_offset = last_position_y - current_position_y;

	last_position_x = current_position_x;
	last_position_y = current_position_y;
}

void mouse_callback(GLFWwindow* window, double x, double y) {
}
