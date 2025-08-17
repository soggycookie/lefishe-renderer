#include "framebuffer.h"

using namespace Lefishe;

Framebuffer::Framebuffer(UINT width, UINT height)
	: m_viewport_width(width), m_viewport_height(height)
{
	create();
}

void Framebuffer::create(){
	glCreateFramebuffers(1, &m_fbo);
}

void Framebuffer::attachColor(){
	m_color = std::make_shared<Texture2D>(
		Texture2DInfo{
			.internal_format = TextureInternalFormat::RGBA8,
			.width = m_viewport_width,
			.height = m_viewport_height,
			.total_mip_levels = 1
		}
	);
	glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_color->id(), 0);
	//glNamedFramebufferRenderbuffer(m_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_color->id());
}

void Framebuffer::attachDepth(bool use_stencil){
	if(use_stencil){
		m_depth = std::make_shared<Texture2D>(
			Texture2DInfo{
				.internal_format = TextureInternalFormat::DEPTH24_STENCIL8,
				.width = m_viewport_width,
				.height = m_viewport_height,
				.total_mip_levels = 1
				}
		);
		//glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth->id());
		glNamedFramebufferTexture(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, m_depth->id(), 0);

	}else{
		m_depth = std::make_shared<Texture2D>(
			Texture2DInfo{
				.internal_format = TextureInternalFormat::DEPTH24_STENCIL8,
				.width = m_viewport_width,
				.height = m_viewport_height,
				.total_mip_levels = 1
				}
		);

		//glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth->id());
		glNamedFramebufferTexture(m_fbo, GL_DEPTH_ATTACHMENT, m_depth->id(), 0);
	}
}

void Framebuffer::finalize(){
	if(glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		LOG_ERROR("Framebuffer is not complete!");
	}else{
		LOG_TRACE("Framebuffer is complete!");
	}
}

void Framebuffer::bind() const{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::release(){
	glDeleteFramebuffers(1, &m_fbo);

}

void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture2D> Framebuffer::colorMap() const{
	return m_color;
}

std::shared_ptr<Texture2D> Framebuffer::depthMap() const{
	return m_depth;
}