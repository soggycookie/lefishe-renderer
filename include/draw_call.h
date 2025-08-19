#pragma once
#include "global_header.h"
#include "buffer.h"
#include "component.h"

namespace Lefishe{


	struct DrawElementIndirectCommand{
		UINT count          = 0;
		UINT instance_count = 0;
		UINT first_index    = 0;
		UINT base_vertex    = 0;
		UINT base_instance  = 0;
	};


	class DrawCall{
	public:
		DrawCall() = default;
		DrawCall(Buffer vertex, Buffer indices, MeshRendererComponent& mesh_renderer,  SIZE_T indirect_offset);	
		
		static inline void unbind(){
			glBindVertexArray(0);
		}

		void bind() const;
		void cleanUpBinding() const;
		SIZE_T indirectOffset() const;
		
		const Buffer& vertexBuffer() const;
		const Buffer& indexBuffer() const;


	private:

		void setup();

		static inline UINT currentID(){
			return m_current_id;
		}


	private:
		UINT m_vao_id = 0;
		//std::vector<Buffer> m_buffer;
		//std::vector<
		//std::vector<DrawElementIndirectCommand> m_cmd;
		Buffer m_vertex_buffer;
		Buffer m_index_buffer;
		MeshRendererComponent& m_mesh_renderer;
		SIZE_T m_indirect_offset = 0;

		static UINT m_current_id;
	};
}