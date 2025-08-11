#pragma once
#include "global_header.h"
#include "id_generator.h"

namespace Lefishe{

	class Program;

	class Material{
	public:
		Material() = default;
		Material(std::shared_ptr<Program> program);

		UINT id() const;

		const std::shared_ptr<Program> program() const;

		void use() const;

		void setUniform(STRING name, const void* data);

	private:

	private:
		UINT m_id = 0;
		std::weak_ptr<Program> m_program;
		std::unordered_map<STRING, const void*> m_uniform_data;
	};


}