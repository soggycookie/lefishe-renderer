#pragma once
#include "global_header.h"
#include "program_manager.h"
#include "material.h"

namespace Lefishe{

	class MaterialManager{
	public:

		MaterialManager(const std::shared_ptr<ProgramManager>& program_manager);
		void addMaterial(std::shared_ptr<Material> mat);
		
		std::shared_ptr<Material> defaultMaterial();

	private:
		std::weak_ptr<ProgramManager> m_program_manager;
		std::vector<std::shared_ptr<Material>> m_materials;
	};

}

