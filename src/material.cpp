#include "material.h"
#include "program.h"

using namespace Lefishe;

Material::Material(std::shared_ptr<Program> program)
	: m_id(IDGenerator::Generate()), m_program(program)
{
}

UINT Material::id() const{
	return m_id;
}

const std::shared_ptr<Program> Material::program() const{
	return m_program.lock();
}

void Material::use() const{
	auto p = program();

	p->bind();
}

void Material::setUniform(STRING name, const void* data){
	m_uniform_data[name] = data;
	
	auto p = program();
	
	use();
	p->setUniform(name, data);
}