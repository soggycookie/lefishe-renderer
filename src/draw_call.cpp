#include "draw_call.h"
#include "component.h"
#include "object.h"

using namespace Lefishe;

UINT DrawCall::m_current_id = 0;

DrawCall::DrawCall(Buffer vertex, Buffer indices, std::shared_ptr<MeshRendererComponent> mesh_renderer,  SIZE_T indirect_offset)
	: m_vertex_buffer(vertex), m_index_buffer(indices), m_mesh_renderer(mesh_renderer), m_indirect_offset(indirect_offset)
{
	setup();
}
		


void DrawCall::bind() const
{
	if(auto renderer = m_mesh_renderer.lock()){
		std::shared_ptr<Object> owner = renderer->owner();
		
		renderer->material()->setUniformData("model", sizeof(MAT4), &(owner->transform()->localToWorldMtx()));
		//int a[2] = {1, 1};
		//renderer->material()->setUniformData("here", sizeof(INT) * 2, a);
	}



	if(auto mesh_renderer = m_mesh_renderer.lock()){
		mesh_renderer->material()->bindAndSetUniform();
	}

	m_current_id = m_vao_id;

	glBindVertexArray(m_vao_id);

}

SIZE_T DrawCall::indirectOffset() const{
	return m_indirect_offset;
}


const Buffer& DrawCall::vertexBuffer() const{
	return m_vertex_buffer;
}

const Buffer& DrawCall::indexBuffer() const{
	return m_index_buffer;
}

void DrawCall::setup(){
	//fixed layout for mesh
	//TODO: dynamic layout for different usage: skinned mesh, post processing...
	//TODO: handle mutiple bind index
	glCreateVertexArrays(1, &m_vao_id);

	const UINT bind_index = 0;
	const SIZE_T offset = 0;
	const SIZE_T stride = 0;

    const GLsizei pos_stride = 3 * sizeof(GLfloat); // 12 bytes (3 floats for position)
    const GLsizei normal_stride = 3 * sizeof(GLfloat); // 12 bytes (3 floats for normal)
    const GLsizei tangent_stride = 3 * sizeof(GLfloat); // 12 bytes (3 floats for tangent)
    const GLsizei color_stride = 4 * sizeof(GLfloat); // 16 bytes (4 floats for color)
    const GLsizei uv_stride = 2 * sizeof(GLfloat); // 8 bytes (2 floats for UV)
	
	if(auto mesh = m_mesh_renderer.lock()->mesh()){
		glVertexArrayVertexBuffer(m_vao_id, 0, m_vertex_buffer.id(), mesh->vertexByteOffset(), pos_stride); // Positions
		glVertexArrayVertexBuffer(m_vao_id, 1, m_vertex_buffer.id(), mesh->normalByteOffset(), normal_stride); // Normals
		glVertexArrayVertexBuffer(m_vao_id, 2, m_vertex_buffer.id(), mesh->tangentByteOffset(), tangent_stride); // Tangents
		glVertexArrayVertexBuffer(m_vao_id, 3, m_vertex_buffer.id(), mesh->vertexColorByteOffset(), color_stride); // Colors
		glVertexArrayVertexBuffer(m_vao_id, 4, m_vertex_buffer.id(), mesh->uvByteOffset(), uv_stride); // UVs
		
		
		glVertexArrayElementBuffer(m_vao_id, m_index_buffer.id());
	

		//pos
		glEnableVertexArrayAttrib(m_vao_id, 0);
		//normal
		glEnableVertexArrayAttrib(m_vao_id, 1);
		//tangent
		glEnableVertexArrayAttrib(m_vao_id, 2);
		//color
		glEnableVertexArrayAttrib(m_vao_id, 3);
		//uv
		glEnableVertexArrayAttrib(m_vao_id, 4);

		glVertexArrayAttribFormat(m_vao_id, 0 , 3, GL_FLOAT, GL_FALSE, mesh->vertexByteOffset());
		glVertexArrayAttribFormat(m_vao_id, 1 , 3, GL_FLOAT, GL_FALSE, mesh->normalByteOffset());
		glVertexArrayAttribFormat(m_vao_id, 2 , 3, GL_FLOAT, GL_FALSE, mesh->tangentByteOffset());
		glVertexArrayAttribFormat(m_vao_id, 3 , 4, GL_FLOAT, GL_FALSE, mesh->vertexColorByteOffset());
		glVertexArrayAttribFormat(m_vao_id, 4 , 2, GL_FLOAT, GL_FALSE, mesh->uvByteOffset());
	
		glVertexArrayAttribBinding(m_vao_id, 0, 0); 
		glVertexArrayAttribBinding(m_vao_id, 1, 1); 
		glVertexArrayAttribBinding(m_vao_id, 2, 2); 
		glVertexArrayAttribBinding(m_vao_id, 3, 3); 
		glVertexArrayAttribBinding(m_vao_id, 4, 4); 

	}else{
		LOG_ERROR("Mesh Component is expired");
		return;
	}

}

