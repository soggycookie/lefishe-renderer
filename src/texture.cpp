#include "texture.h"

using namespace Lefishe;


//Base Texture

void Texture::bind(UINT index) const{
	glBindTextureUnit(index, m_tex_id);
}

UINT Texture::id() const{
	return m_tex_id;
}


//Render Texture

//RenderTexture::RenderTexture(Texture2DInfo info)
//	:m_info(info)
//{
//	create();
//}
//
//void RenderTexture::create(){
//	glCreateRenderbuffers(1, &m_tex_id);
//
//	glNamedRenderbufferStorage(m_tex_id,  static_cast<int>(m_info.internal_format), m_info.width, m_info.height);
//}
//
//void RenderTexture::create(Texture2DInfo info){
//	
//	if(m_info != Texture2DInfo()){
//		LOG_WARN("Texture info is already assigned");
//		return;
//	}
//	
//	m_info = info;
//
//	create();
//}




//Texture 2D


Texture2D::Texture2D(Texture2DInfo info)
	: m_info(info)
{
	create();
	samplerMode();
}

void Texture2D::create(){
	glCreateTextures(GL_TEXTURE_2D, 1, &m_tex_id);
	

	glTextureStorage2D(m_tex_id, m_info.total_mip_levels, static_cast<int>(m_info.internal_format), m_info.width, m_info.height);
	
}

void Texture2D::subDataTex2D(UINT mip_level, INT x_offset, INT y_offset, UINT width, UINT height, TexturePixelFormat format, TexturePixelType type, const void* data  ){
	glTextureSubImage2D(m_tex_id, mip_level, x_offset, y_offset, width, height, static_cast<int>(format), static_cast<int>(type), data );
}

	
void Texture2D::samplerMode(){
	glTextureParameteri(m_tex_id, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTextureParameteri(m_tex_id, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTextureParameteri(m_tex_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_tex_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



