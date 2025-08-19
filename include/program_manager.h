#pragma once
#include "global_header.h"
#include "program.h"

#define DEFAULT_PROGRAM "common"
#define SCREEN_SPACE_PROGRAM "ssquad"

namespace Lefishe{



class ProgramManager{
public:
	void add(const std::pair<STRING, std::shared_ptr<Program>>& program);
	void add(const std::unordered_map<STRING, std::shared_ptr<Program>>& programs);

	std::shared_ptr<Program> getProgram(STRING key);

private:
	std::unordered_map<STRING, std::shared_ptr<Program>> m_programs;

};

}