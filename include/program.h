#pragma once
#include "global_header.h"

namespace Lefishe {
	
	class Shader;

	class Program {
	public:
		Program();

		void create();
		void attach(const Shader& shader);

		void link() const;
		void bind() const;
		void clear();

	private:
		UINT m_id;
		std::array<Shader, 4> m_shader_objs;

	};


	class ProgramFactory {
	public:
		static std::shared_ptr<Program> createProgram(const std::string& file);

	};

}