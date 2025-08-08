#pragma once
#include "global_header.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Lefishe {

	struct ShaderSource {
		STRING vert;
		STRING frag;
		STRING geo;
		STRING compute;
	};


	enum class PipelineStage {
		NONE = -1,
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		COMPUTE = GL_COMPUTE_SHADER
	};

	class Shader {
	public:


		Shader() = default;
		Shader(STRING& src, PipelineStage type);

		UINT id() const;
		STRING getStageStr() const;

	private:
		void clear();
		void compile(const STRING& src);

	private:
		PipelineStage m_stage = PipelineStage::NONE;
		UINT m_id = 0;
	};

	class ShaderLoader {
	public:
		static void parseShader(const STRING & path, ShaderSource & src_out);
	};

}