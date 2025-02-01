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

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define PLANE_SIZE	  100
#define SUB_DIVISION  64

#define Min(a, b) (((a) < (b)) ? a : b)
#define Max(a, b) (((a) > (b)) ? a : b)

void generate_plane_vertices(float size, uint32_t sub_division, float *vertices, uint32_t *indices);
void get_mouse_offset(GLFWwindow *window, float *x_offset, float *y_offset);

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

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Simple renderer", NULL, NULL);
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	logger_set_level(LOG_LEVEL_DEBUG);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Renderer *gl_renderer = renderer_create(BACKEND_API_OPENGL);

	/**
	 * ===========================================================================================
	 * -------- Vertex array object creation
	 * ===========================================================================================
	 **/
	float vertices[((SUB_DIVISION + 2) * (SUB_DIVISION + 2)) * 5];
	uint32_t indices[((SUB_DIVISION + 1) * (SUB_DIVISION + 1)) * 6];
	generate_plane_vertices(PLANE_SIZE, SUB_DIVISION, vertices, indices);

	// Create vertex array object
	VertexAttribute attributes[] = {
		{ .name = "a_position", .format = FORMAT_FLOAT3 },
		{ .name = "a_uv", .format = FORMAT_FLOAT2 },
	};
	Buffer *vertex_buffer = gl_renderer->buffer_create(gl_renderer, BUFFER_TYPE_VERTEX, sizeof(vertices), vertices);
	gl_renderer->buffer_set_layout(gl_renderer, vertex_buffer, attributes, 2);
	Buffer *index_buffer = gl_renderer->buffer_create(gl_renderer, BUFFER_TYPE_INDEX, sizeof(indices), indices);

	/**
	 * ===========================================================================================
	 * -------- Texture creation
	 * ===========================================================================================
	 **/

	const char *paths[] = { "assets/textures/container.jpg", "assets/textures/awesomeface.png" };
	Texture *texture0 = gl_renderer->texture_load(gl_renderer, paths[0]);
	Texture *texture1 = gl_renderer->texture_load(gl_renderer, paths[1]);

	/**
	 * ===========================================================================================
	 * -------- Shader creation
	 * ===========================================================================================
	 **/
	Shader *shader = gl_renderer->shader_from_file("assets/shaders/vertex_shader.glsl", "assets/shaders/fragment_shader.glsl", NULL);
	gl_renderer->shader_activate(shader);
	gl_renderer->shader_seti(shader, "u_texture_1", 0);
	gl_renderer->shader_seti(shader, "u_texture_2", 1);

	/**
	 * ===========================================================================================
	 * -------- Math
	 * ===========================================================================================
	 **/

	mat4 model, view, projection;

	glm_mat4_identity(model);

	glm_mat4_identity(view);
	glm_translate(view, (vec3){ 0.0f, 0.0f, -5.0f });

	glm_mat4_identity(projection);
	glm_perspective(glm_rad(45.f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f, projection);

	/**
	 * ===========================================================================================
	 * -------- Camera
	 * ===========================================================================================
	 **/

	Camera *camera = camera_create();
	camera_set_perspective(camera, glm_rad(45.0f), 0.1f, 100.f);

	vec3 camera_position = { 0.f, 5.f, 5.0f },
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

		gl_renderer->shader_activate(shader);
		gl_renderer->shader_set4fm(shader, "u_model", (float *)model);
		gl_renderer->shader_set4fm(shader, "u_view", camera_get_view(camera));
		gl_renderer->shader_set4fm(shader, "u_projection", camera_get_projection(camera));

		// gl_renderer->texture_activate(gl_renderer, texture0, 0);
		// gl_renderer->texture_activate(gl_renderer, texture1, 1);
		gl_renderer->draw_indexed(gl_renderer, vertex_buffer, index_buffer, ((SUB_DIVISION + 1) * (SUB_DIVISION + 1)) * 6);
	}

	gl_renderer->shader_destroy(shader);
	gl_renderer->buffer_destroy(gl_renderer, vertex_buffer);
	renderer_destroy(gl_renderer);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void get_mouse_offset(GLFWwindow *window, float *x_offset, float *y_offset) {
	static uint32_t current_frame = 0;
	static double last_position_x = 0.0f, last_position_y = 0.0f;

	double current_position_x, current_position_y;
	glfwGetCursorPos(window, &current_position_x, &current_position_y);

	if (current_frame == 0) {
		last_position_x = WINDOW_WIDTH / 2.f;
		last_position_y = WINDOW_HEIGHT / 2.f;
		current_frame++;
		return;
	}

	*x_offset = current_position_x - last_position_x;
	*y_offset = last_position_y - current_position_y;

	last_position_x = current_position_x;
	last_position_y = current_position_y;
}

void generate_plane_vertices(float size, uint32_t sub_division, float *vertices, uint32_t *indices) {
	uint32_t rows, columns, quad_count;
	rows = columns = 2 + sub_division;
	quad_count = sub_division + 1;
	uint32_t vertices_ptr = 0, indices_ptr = 0;

	for (uint32_t z = 0; z < rows; z++) {
		for (uint32_t x = 0; x < columns; x++) {
			uint32_t index = x + z * columns;
			// Position
			vertices[vertices_ptr++] = -size / 2.f + x * ((float)size / quad_count);
			vertices[vertices_ptr++] = 0.0f;
			vertices[vertices_ptr++] = -size / 2.f + z * ((float)size / quad_count);

			// UV
			vertices[vertices_ptr++] = (float)x / quad_count;
			vertices[vertices_ptr++] = (float)z / quad_count;

			if (x < columns - 1 && z < rows - 1) {
				indices[indices_ptr++] = index + 1;
				indices[indices_ptr++] = index;
				indices[indices_ptr++] = index + columns;

				indices[indices_ptr++] = index + 1;
				indices[indices_ptr++] = index + columns;
				indices[indices_ptr++] = index + columns + 1;
			}
		}
	}
}
