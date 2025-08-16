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
	glCreateRenderbuffers(1, &m_color_rbo);
    glNamedRenderbufferStorage(m_color_rbo, GL_RGBA8, m_viewport_width, m_viewport_height);
	glNamedFramebufferRenderbuffer(m_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_color_rbo);
}

void Framebuffer::attachDepth(bool use_stencil){
	glCreateRenderbuffers(1, &m_depth_rbo);
	if(use_stencil){
		glNamedRenderbufferStorage(m_depth_rbo, GL_DEPTH24_STENCIL8, m_viewport_width, m_viewport_height);
		glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_rbo);
	}else{
		glNamedRenderbufferStorage(m_depth_rbo, GL_DEPTH_COMPONENT24, m_viewport_width, m_viewport_height);
		glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depth_rbo);
	}
}

void Framebuffer::finalize(){
	if(glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		LOG_ERROR("Framebuffer is not complete!");
	}else{
		LOG_TRACE("Framebuffer is complete!");
	}
}

void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::release(){
	glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_color_rbo);
    glDeleteRenderbuffers(1, &m_depth_rbo);
}

void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}