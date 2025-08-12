#include "material.h"
#include "program.h"

using namespace Lefishe;

Material::Material(std::shared_ptr<Program> program)
	: m_id(IDGenerator::Generate()), m_program(program)
{
	getUniform();
}

UINT Material::id() const{
	return m_id;
}

const std::shared_ptr<Program> Material::program() const{
	return m_program.lock();
}

void Material::bindAndSetUniform() const{
	auto p = program();

	p->bind();

	for(const auto& [key, value] : m_uniform_data){
		p->setUniform(key, value);
	}
}

void Material::setUniformData(STRING name, const void* data){
	if(m_uniform_data.contains(name)){
		m_uniform_data[name] = data;
	}
}

void Material::getUniform(){
	auto p = program();

	for(const auto& [key, value] : p->uniform()){
		m_uniform_data[key] = nullptr;
	}
}
