#include "material_manager.h"

using namespace Lefishe;

MaterialManager::MaterialManager(const std::shared_ptr<ProgramManager>& program_manager)
	: m_program_manager(program_manager)
{
}

void MaterialManager::addMaterial(std::shared_ptr<Material> mat){
	m_materials.push_back(mat);
}

std::shared_ptr<Material> MaterialManager::defaultMaterial(){
	if(auto p = m_program_manager.lock()){
		if(p->getProgram(DEFAULT_PROGRAM) == nullptr){
			auto s = ProgramFactory::createProgram("resource/shader/common.shader");
			
			if(s == nullptr){
				return nullptr;
			}
			
			p->add(
				{
					DEFAULT_PROGRAM, s 
				}
			);
		}
		
		return std::make_shared<Material>(p->getProgram(DEFAULT_PROGRAM));
	}

	return nullptr;
}