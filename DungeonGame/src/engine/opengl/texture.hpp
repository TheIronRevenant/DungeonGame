#pragma once

#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, unsigned int> TextureMap;

namespace DG
{
	unsigned int loadOpaqueTexture(const char* fileName);
	unsigned int loadTransparentTexture(const char* fileName);

	//Global used for storing textures
	extern TextureMap textures;
}