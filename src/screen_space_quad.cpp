#include "screen_space_quad.h"
#include "material.h"
#include "buffer.h"

using namespace Lefishe;

constexpr FLOAT screen_space_quad_vertices[] = {
	// positions    // UVs
	-1.0f, -1.0f,   0.0f, 0.0f,  // bottom-left
	 1.0f, -1.0f,   1.0f, 0.0f,  // bottom-right
	 1.0f,  1.0f,   1.0f, 1.0f,  // top-right
	-1.0f,  1.0f,   0.0f, 1.0f   // top-left
};

constexpr UINT screen_space_quad_indices[] = {
	0, 1, 2,   // first triangle
	0, 2, 3    // second triangle
};

ScreenSpaceQuad::ScreenSpaceQuad(){
	create();
}

void ScreenSpaceQuad::create(){
	m_vertex_buffer.create(sizeof(screen_space_quad_vertices), screen_space_quad_vertices);
	m_index_buffer.create(sizeof(screen_space_quad_indices), screen_space_quad_indices);

	glCreateVertexArrays(1, &m_vao_id);
	glVertexArrayVertexBuffer(m_vao_id, 0, m_vertex_buffer.id(), 0 , sizeof(FLOAT) * 4);

	glVertexArrayElementBuffer(m_vao_id, m_index_buffer.id());

	glEnableVertexArrayAttrib(m_vao_id, 0);
	glEnableVertexArrayAttrib(m_vao_id, 1);

	glVertexArrayAttribFormat(m_vao_id, 0 , 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(m_vao_id, 1 , 2, GL_FLOAT, GL_FALSE, sizeof(FLOAT) * 2);

	glVertexArrayAttribBinding(m_vao_id, 0, 0); 
	glVertexArrayAttribBinding(m_vao_id, 1, 0); 
}

void ScreenSpaceQuad::release(){
	
}

void ScreenSpaceQuad::bind(){
	glBindVertexArray(m_vao_id);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}