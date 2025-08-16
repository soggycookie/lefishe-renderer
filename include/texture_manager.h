#pragma once
#include "global_header.h"
#include "texture.h"
#include "texture_loader.h"

namespace Lefishe{

class TextureManager{
public:
	std::shared_ptr<Texture> createTexture(const char* path);

	std::shared_ptr<Texture> getTexture(const STRING& path);

private:
	std::unordered_map<STRING, std::shared_ptr<Texture>> m_textures;
	TextureLoader m_loader;
};

}