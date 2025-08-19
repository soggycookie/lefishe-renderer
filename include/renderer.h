#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "draw_call.h"
#include "object.h"

#include "screen_space_quad.h"

namespace Lefishe {

	struct RendererConfig {
		
	};

	class Renderer {
	public:
		Renderer();
		void draw(DrawCall& drawcall);


	private:
		//ScreenSpaceQuad m_quad;

	};

}
