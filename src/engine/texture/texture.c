#include "texture.h"

void texture_load(struct texture* texture, const char* path)
{
	/* if (texture->loaded) return; */

	logger_log_string(LOG, "Loading texture: ");
	printf("%s\n", path);

	stbi_set_flip_vertically_on_load(1);	

	texture->data = stbi_load(path, &texture->width, &texture->height, &texture->channels, 0);

	if (!texture_is_good(texture))
	{
		logger_log_string(ERROR, "Failed to load texture: " );
		printf("%s file exists?\n", path);
		exit(EXIT_FAILURE);
	}
	else 
	{
		texture->file_path = path;
		// Generate and bind texture
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		// Set texture parameters of currently bound texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (texture->channels == 3) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else if (texture->channels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			logger_log_string(ERROR, "Texture uknown file format: ");
			printf("%s\n", path);
			exit(EXIT_FAILURE);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// We can free the texture data, wont be using it anymore
	free(texture->data);

	logger_log_string(SUCCESS, "Texture loaded successfully.\n");

	texture->loaded = 1;
}

unsigned int texture_get_id(struct texture texture)
{ return texture.id; }

void texture_bind(struct texture texture) 
{ glBindTexture(GL_TEXTURE_2D, texture.id); }

void texture_unbind(void)
{ glBindTexture(GL_TEXTURE_2D, 0); }

const char* texture_get_path(struct texture texture)
{ return texture.file_path; }

unsigned char* texture_get_data(struct texture texture)
{ return texture.data; } 

int texture_get_width(struct texture texture)
{ return texture.width; }

int texture_get_height(struct texture texture)
{ return texture.height; } 

void texture_free(struct texture* texture)
{ glDeleteTextures(1, &texture->id); texture->loaded = 0; }

int texture_is_good(struct texture* texture)
{ return texture->data != 0; } 

