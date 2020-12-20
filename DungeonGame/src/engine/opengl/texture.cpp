#include "texture.hpp"
#include "../logger.hpp"

#include "../stb/stb_image.h"
#include <GL/glew.h>

TextureMap DG::textures;

unsigned int DG::loadOpaqueTexture(const char* fileName)
{
	unsigned int texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DG::log << "Failed to load " << fileName << "\n";
		if (strncmp("resources\\textures\\default.png", fileName, strlen(fileName)))
		{
			DG::log << "Failed to load default.png\n";
			DG::dumpLog();
			exit(-6);
		}
		else
		{
			return 1;
		}
	}

	stbi_image_free(data);

	return texture;
}

unsigned int DG::loadTransparentTexture(const char* fileName)
{
	unsigned int texture = 0;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		DG::log << "Failed to load " << fileName << "\n";
		if (strncmp("resources\\textures\\default.png", fileName, strlen(fileName)))
		{
			DG::log << "Failed to load default.png\n";
			DG::dumpLog();
			exit(-6);
		}
		else
		{
			return 1;
		}
	}

	stbi_image_free(data);

	return texture;
}