#pragma once
#include "global_header.h"

namespace Lefishe{

	class Framebuffer{
	public:
		Framebuffer(UINT width, UINT height);

		static void unbind();

		void attachColor();
		void attachDepth(bool use_stencil);
		void finalize();
		void bind();
	private:
		void create();
		void release();

	private:
		UINT m_fbo;
		UINT m_color_rbo;
		UINT m_depth_rbo;

		UINT m_viewport_width = 0;
		UINT m_viewport_height = 0;
	};

}
