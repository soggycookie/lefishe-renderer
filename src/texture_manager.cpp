#include "texture_manager.h"


using namespace Lefishe;


std::shared_ptr<Texture2D> TextureManager::createTexture(const char* path){
	if(m_textures.contains(path)){
		LOG_INFO("Texture at {0} already created!\n", path);
		return nullptr;
	}

	auto tex = m_loader.load(path);
	
	if(!tex){
		return nullptr;
	}

	m_textures.insert({path, tex});

	return tex;
}

std::shared_ptr<Texture2D> TextureManager::getTexture(const STRING& path){
	if(!m_textures.contains(path)){
		return nullptr;
	}

	return m_textures.at(path);
}