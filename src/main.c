#include "base.h"
#include "renderer.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cglm/affine-mat.h>
#include <cglm/cglm.h>
#include <stb/stb_image.h>

#include <stdint.h>
#include <stdio.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define Min(a, b) (((a) < (b)) ? a : b)
#define Max(a, b) (((a) > (b)) ? a : b)

void generate_cube_vertices(float* vertices);

void get_mouse_offset(GLFWwindow* window, float* x_offset, float* y_offset);

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

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple renderer", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	logger_set_level(LOG_LEVEL_DEBUG);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Renderer* gl_renderer = renderer_create(BACKEND_API_OPENGL);

	/**
	 * ===========================================================================================
	 * -------- Vertex array object creation
	 * ===========================================================================================
	 **/
	float vertices[36 * 5];
	generate_cube_vertices(vertices);

	// Create vertex array object
	uint32_t vertex_array_object = 0;
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	Buffer* vertex_buffer = gl_renderer->buffer_create(gl_renderer, BUFFER_TYPE_VERTEX, sizeof(vertices), vertices);

	gl_renderer->buffer_activate(gl_renderer, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * (sizeof *vertices), (void*)(3 * (sizeof *vertices)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	gl_renderer->buffer_deactivate(gl_renderer, vertex_buffer);

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
		LOG_ERROR("TEXTURE:FILE [ %s ] NOT_FOUND", paths[0]);
		return 1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// Second texture
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	data = stbi_load(paths[1], &width, &height, &channel_count, 0);
	if (!data) {
		LOG_ERROR("TEXTURE:FILE [ %s ] NOT_FOUND", paths[1]);
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
	Shader* shader = gl_renderer->shader_from_file("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl", NULL);
	gl_renderer->shader_activate(shader);
	gl_renderer->shader_seti(shader, "u_texture_1", 0);
	gl_renderer->shader_seti(shader, "u_texture_2", 1);

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

	Camera* camera = camera_create();
	camera_set_perspective(camera, glm_rad(45.0f), 0.1f, 100.f);

	vec3 camera_position = { 0.f, 0.f, 5.0f },
		 camera_up = { 0.0f, 1.0f, 0.0f }, camera_front = { 0.0f, 0.0f, -1.0f }, camera_right = { 1.0f, 0.0f, 0.0f };
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

		const float sensitivity = 4.f;

		yaw += x_offset * delta_time * sensitivity;
		pitch += y_offset * delta_time * sensitivity;
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

		camera_update(camera, camera_position, camera_front, camera_up);

		glClearColor(0.95f, .95f, .95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float time = glfwGetTime();
		vec4 u_color = { (cos(time) / 2.f) + .5f, (sin(time) / 2.f) + .5f, 0.0f, 1.0f };

		gl_renderer->shader_set4fv(shader, "u_color", u_color);
		gl_renderer->shader_set4fm(shader, "u_view", camera_get_view(camera));
		gl_renderer->shader_set4fm(shader, "u_projection", camera_get_projection(camera));

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
			gl_renderer->shader_set4fm(shader, "u_model", (float*)model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
	}

	gl_renderer->shader_destroy(shader);
	renderer_destroy(gl_renderer);
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

void generate_cube_vertices(float* vertices) {
	int vertexIndex = 0;

	// Define the positions of the cube corners relative to the center (0,0,0)
	float cubeCorners[8][3] = {
		{ -0.5f, -0.5f, -0.5f }, // 0: Back Bottom Left (BBL)
		{ 0.5f, -0.5f, -0.5f }, // 1: Back Bottom Right (BBR)
		{ 0.5f, 0.5f, -0.5f }, // 2: Back Top Right (BTR)
		{ -0.5f, 0.5f, -0.5f }, // 3: Back Top Left (BTL)
		{ -0.5f, -0.5f, 0.5f }, // 4: Front Bottom Left (FBL)
		{ 0.5f, -0.5f, 0.5f }, // 5: Front Bottom Right (FBR)
		{ 0.5f, 0.5f, 0.5f }, // 6: Front Top Right (FTR)
		{ -0.5f, 0.5f, 0.5f } // 7: Front Top Left (FTL)
	};

	// Define texture coordinates for each vertex in a face (consistent for all faces)
	float textureCoords[4][2] = {
		{ 0.0f, 0.0f }, // Bottom Left (BL)
		{ 1.0f, 0.0f }, // Bottom Right (BR)
		{ 1.0f, 1.0f }, // Top Right (TR)
		{ 0.0f, 1.0f } // Top Left (TL)
	};

	// Define the vertex indices for each face, in counter-clockwise order for each triangle
	int faceIndices[6][6] = {
		{ 0, 1, 2, 2, 3, 0 }, // Back face
		{ 4, 5, 6, 6, 7, 4 }, // Front face
		{ 7, 3, 0, 0, 4, 7 }, // Left face
		{ 1, 5, 6, 6, 2, 1 }, // Right face
		{ 0, 1, 5, 5, 4, 0 }, // Bottom face
		{ 3, 2, 6, 6, 7, 3 } // Top face
	};

	// Loop through each face of the cube (6 faces)
	for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
		// Loop through the 6 vertices for each face (2 triangles)
		for (int vertexInFace = 0; vertexInFace < 6; vertexInFace++) {
			int cornerIndex = faceIndices[faceIndex][vertexInFace];

			// Vertex position (x, y, z)
			vertices[vertexIndex++] = cubeCorners[cornerIndex][0];
			vertices[vertexIndex++] = cubeCorners[cornerIndex][1];
			vertices[vertexIndex++] = cubeCorners[cornerIndex][2];

			// Texture coordinates (u, v)
			int textureCoordIndex;
			if (vertexInFace == 0 || vertexInFace == 5)
				textureCoordIndex = 0; // BL
			else if (vertexInFace == 1)
				textureCoordIndex = 1; // BR
			else if (vertexInFace == 2 || vertexInFace == 3)
				textureCoordIndex = 2; // TR
			else
				textureCoordIndex = 3; // vertexInFace == 4, TL

			vertices[vertexIndex++] = textureCoords[textureCoordIndex][0]; // u
			vertices[vertexIndex++] = textureCoords[textureCoordIndex][1]; // v
		}
	}
}
