#include "texture.h"

using namespace Lefishe;

Texture::Texture(TextureInfo info)
	: m_info(info)
{
	create();
	samplerMode();
}

void Texture::create(){
	glCreateTextures(static_cast<int>(m_info.type), 1, &m_id);
	
	switch(m_info.type){
	case TextureType::TEX_2D:
		glTextureStorage2D(m_id, m_info.total_mip_levels, static_cast<int>(m_info.internal_format), m_info.width, m_info.height);
	}
}

void Texture::subDataTex2D(UINT mip_level, INT x_offset, INT y_offset, UINT width, UINT height, TexturePixelFormat format, TexturePixelType type, const void* data  ){
	if(m_info.type != TextureType::TEX_2D){
		LOG_WARN("[TEXTURE] The type of texture is not tex 2D!");
	}

	glTextureSubImage2D(m_id, mip_level, x_offset, y_offset, width, height, static_cast<int>(format), static_cast<int>(type), data );
}

	
void Texture::samplerMode(){
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::bind(UINT index){
	glBindTextureUnit(index, m_id);
}


