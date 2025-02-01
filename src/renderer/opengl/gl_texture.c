#include "base.h"
#include "gl_types.h"

#include <glad/gl.h>
#include <stb/stb_image.h>
#include <stdlib.h>

Texture *opengl_texture_load(struct _renderer *self, const char *texture_path) {
	OpenGLTexture *texture = malloc(sizeof(OpenGLTexture));

	glGenTextures(1, &texture->id);

	// First texture
	glBindTexture(GL_TEXTURE_2D, texture->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);

	int32_t width, height, channel_count;
	uint8_t *data = stbi_load(texture_path, &width, &height, &channel_count, 0);
	if (!data) {
		LOG_ERROR("Texture path [ %s ] not found", texture_path);
		exit(1);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channel_count == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	texture->width = width;
	texture->height = height;
	texture->channels = channel_count;
	texture->path = texture_path;
	return texture;
}
void opengl_texture_destroy(struct _renderer *self, Texture *texture) {
	if (texture) {
		OpenGLTexture *gl_texture = (OpenGLTexture *)texture;
		glDeleteTextures(1, &gl_texture->id);
		free(texture);
	}
}

void opengl_texture_activate(struct _renderer *self, Texture *texture, uint32_t texture_unit) {
	if (texture == NULL) {
		LOG_ERROR("Null texture passed to texture_activate!");
		exit(1);
	}
	OpenGLTexture *gl_texture = (OpenGLTexture *)texture;

	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(GL_TEXTURE_2D, gl_texture->id);
}
