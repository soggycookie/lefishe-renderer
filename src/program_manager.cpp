#include "program_manager.h"


using namespace Lefishe;

void ProgramManager::add(const std::pair<STRING, std::shared_ptr<Program>>& program){
	m_programs.insert(program);
}

void ProgramManager::add(const std::unordered_map<STRING, std::shared_ptr<Program>>& programs){
	for(auto& [key, value] : programs){
		m_programs[key] = value;
	}
}

std::shared_ptr<Program> ProgramManager::getProgram(STRING key){
	return m_programs[key];
}