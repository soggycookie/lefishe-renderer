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
		void setupMainCamera(std::shared_ptr<Object> cam);

	private:
		Buffer m_cam_ubo = Buffer(sizeof(MAT4) * 2);
	};

}
