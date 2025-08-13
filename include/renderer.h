#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "draw_call.h"
#include "object.h"

namespace Lefishe {

	struct RendererConfig {
		
	};

	class Renderer {
	public:
		void draw(const std::vector<DrawCall>& drawcalls);


	private:

	};

}
