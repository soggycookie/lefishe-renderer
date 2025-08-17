#pragma once
#include "global_header.h"
#include "texture.h"

namespace Lefishe{

	class Framebuffer{
	public:
		Framebuffer(UINT width, UINT height);

		static void unbind();

		void attachColor();
		void attachDepth(bool use_stencil);
		void finalize();
		void bind() const;

		std::shared_ptr<Texture2D> colorMap() const;
		std::shared_ptr<Texture2D> depthMap() const;

	private:
		void create();
		void release();

	private:
		UINT m_fbo;
		std::shared_ptr<Texture2D> m_color;
		std::shared_ptr<Texture2D> m_depth;

		UINT m_viewport_width = 0;
		UINT m_viewport_height = 0;
	};

}
