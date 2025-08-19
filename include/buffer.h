#pragma once

#include "global_header.h"

namespace Lefishe{
	
	enum BufferBindTarget{
		UNIFORM        = GL_UNIFORM_BUFFER ,
		SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER
	};

	enum BufferTarget{
		ARRAY    = GL_ARRAY_BUFFER,
		ELEMENT  = GL_ELEMENT_ARRAY_BUFFER,
		INDIRECT = GL_DRAW_INDIRECT_BUFFER
	};

	class Buffer{
	public:
		Buffer() = default;
		Buffer(UINT total_size, const void* data = nullptr);

		static inline void unbind(BufferTarget target){
			glBindBuffer(target, 0);
		}

		void create(const void* data);
		void create(UINT total_size, const void* data);

		void subData(UINT size, SIZE_T offset, const void* data);
		void bindIndex(BufferBindTarget type, int index = 0);

		void bind(BufferTarget target) const;

		UINT id() const;
		UINT size() const;

		void release();
	private:

	private:
		UINT m_id = 0;
		UINT m_size = 0;

	};

}