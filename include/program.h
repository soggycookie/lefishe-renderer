#pragma once
#include "global_header.h"
#include "shader.h"

namespace Lefishe {
	

	class Program {
	public:
		
		struct UniformInfo
		{ 
			GLint location;
			GLsizei count;
			GLenum type;
		};
		
		Program();

		void attach(const Shader& shader);

		void link();
		void bind() const;

		void clear();

		UniformInfo getUniform(const std::string& u) const;
		void setUniform(const std::string& u, const void* data) const;

		const std::unordered_map<std::string, UniformInfo>& uniform() const;

	private:
		void create();
		void retrieveUniforms();

	private:
		UINT m_id;
		std::array<Shader, 4> m_shader_objs;
		std::unordered_map<std::string, UniformInfo> m_uniforms;
	};


	class ProgramFactory {
	public:
		static std::shared_ptr<Program> createProgram(const std::string& path);
	};



}