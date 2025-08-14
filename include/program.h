#pragma once
#include "global_header.h"
#include "shader.h"

namespace Lefishe {
	

	class Program {
	public:
		
		struct UniformInfo
		{ 
		public:
			GLint location;
			GLint count;
			GLenum type;
		public:
			UniformInfo() = default;
			UniformInfo(GLint loc, GLint c, GLenum t);
		};
		
		Program();

		void attach(const Shader& shader);

		void link();
		void bind() const;

		void clear();

		UniformInfo getUniform(const STRING& u) const;
		void setUniform(const STRING& u, const void* data) const;

		const std::unordered_map<STRING, UniformInfo>& uniform() const;

		void getUniformValue(const STRING& name, void* out_data);


	private:
		void create();
		void retrieveUniforms();

		
		void getUniformValue(GLint location, GLint size, FLOAT* out_data);
		void getUniformValue(GLint location, GLint size, INT* out_data);
		void getUniformValue(GLint location, GLint size, UINT* out_data);
		void getUniformValue(GLint location, GLint size, DOUBLE* out_data);

	private:
		UINT m_id;
		std::array<Shader, 4> m_shader_objs;
		std::unordered_map<STRING, UniformInfo> m_uniforms;
	};


	class ProgramFactory {
	public:
		static std::shared_ptr<Program> createProgram(const std::string& path);
	};



}