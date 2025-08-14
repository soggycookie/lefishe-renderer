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
	if(auto p = program()){
    	p->bind();

	    for(const auto& [key, value] : m_uniform_data){
		    p->setUniform(key, value);
	    }
    }
}

void Material::setUniformData(STRING name, SIZE_T size, const void* data){
	if(m_program.expired()){
        return;
    }
    
    if(m_uniform_data.contains(name) && size > 0){
        std::memcpy(m_uniform_data[name], data, size);
	}
}

Material::~Material(){
    for(auto& pair: m_uniform_data){
        std::free(pair.second);
    }  
}

void Material::getUniform(){
	if(m_program.expired()){
        return;
    }

    auto p = program();

	for(const auto& [key, info] : p->uniform()){
        
        const int size = info.count;
        void* data = nullptr;

        switch(info.type){
        case GL_FLOAT:{
            data = std::malloc(sizeof(FLOAT) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        }

        break;
        case GL_FLOAT_VEC2:{
            data = std::malloc(sizeof(VEC2) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        }

        break;
        case GL_FLOAT_VEC3:{
            data = std::malloc(sizeof(VEC3) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        
        }

        break;
        case GL_FLOAT_VEC4:{
            data = std::malloc(sizeof(VEC4) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        }

        break;
        case GL_FLOAT_MAT4:{
            data = std::malloc(sizeof(MAT4) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        }

        break;
        case GL_INT:{
            data = std::malloc(sizeof(INT) * size);

			p->getUniformValue(key, data);
            m_uniform_data[key] = data;

            //INT* b = static_cast<INT*>(data);
            //for(int i =0; i < size; i++){
            //    LOG_INFO("{0}", b[i]);
            //}
        }
            
        break;
        case GL_SAMPLER_2D:{
            data = std::malloc(sizeof(INT) * size);
			p->getUniformValue(key, data);
            m_uniform_data[key] = data;
        }
            //INT* a = static_cast<INT*>(data);
            //LOG_WARN("here");
            //for(int i =0; i < size; i++){
            //    LOG_INFO("{0}", a[i]);
            //}


            break;
        }
	}
}
