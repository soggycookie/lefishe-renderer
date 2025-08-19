#pragma once
#include "global_header.h"
#include "texture.h"
#include "texture_loader.h"

#define DEFAULT_DIFFUSE_PATH "resource/asset/texture/white_default.png"

namespace Lefishe{

class TextureManager{
public:
	std::shared_ptr<Texture2D> createTexture(const char* path);

	std::shared_ptr<Texture2D> getTexture(const STRING& path);

private:
	std::unordered_map<STRING, std::shared_ptr<Texture2D>> m_textures;
	TextureLoader m_loader;
};

}