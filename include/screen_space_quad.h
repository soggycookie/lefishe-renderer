#pragma once
#include "global_header.h"
#include "buffer.h"

namespace Lefishe{

	class Material;

	class ScreenSpaceQuad {
	public:
		ScreenSpaceQuad();
		void bind();

	private:
		void create();
		void release();

	private:
		UINT m_vao_id;
		Buffer m_vertex_buffer;
		Buffer m_index_buffer;
	};

}
