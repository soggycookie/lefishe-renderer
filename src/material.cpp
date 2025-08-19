#include "material.h"
#include "program.h"

using namespace Lefishe;

Material::Material(std::shared_ptr<Program> program, MaterialSettings settings )
	: m_id(IDGenerator::Generate()), m_program(program), m_material_settings(settings)
{
	getUniformInfo();
}

UINT Material::id() const{
	return m_id;
}

const std::shared_ptr<Program> Material::program() const{
	return m_program.lock();
}

void Material::bindAndSetUniform() const{
	if(auto p = program()){
        bindSettings();
    	p->bind();

        for(const auto& [key, texture] : m_textures){
			if(!texture.expired()){
                texture.lock()->bind(getTextureSlot(key));
            }
        }

	    for(const auto& [key, value] : m_uniform_data){
		    p->setUniform(key, value);
	    }
    }
}

void Material::cleanUpBinding() const {
	for(const auto& [key, textures] : m_textures) {
		Texture::unbind(getTextureSlot(key));
	}
}

void Material::setUniformData(const STRING& name, SIZE_T size, const void* data){
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

INT Material::getTextureSlot(const STRING& name) const{
    if(!m_texture_slots.contains(name)){
        return 0;
    }
    
    return m_texture_slots.at(name);
}

void Material::assignTexture(const STRING& name, std::shared_ptr<Texture> texture){
    if(!m_textures.contains(name)){
        return;
    }

    m_textures[name]= texture;
}

void Material::getUniformInfo(){
	if(m_program.expired()){
        return;
    }

    auto p = program();

	for(const auto& [key, info] : p->uniform()){
        
        const int size = info.count;
        void* data = nullptr;
        INT* bind_slots = nullptr;

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
            bind_slots = new INT[1];
			p->getUniformValue(key, bind_slots);
            m_texture_slots[key] = bind_slots[0];

            m_textures[key] = {};

        }

            break;
        }
	}
}



void Material::bindSettings() const{
	glDepthMask(m_material_settings.depth_written ? GL_TRUE : GL_FALSE);
	glDepthFunc(static_cast<INT>(m_material_settings.depth_func));

    if(m_material_settings.is_cull){
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<INT>(m_material_settings.cull_mode));
    }else{
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, static_cast<INT>(m_material_settings.polygon_mode));

    glEnable(GL_BLEND);
    glBlendFunc(static_cast<int>(m_material_settings.src_factor), static_cast<int>(m_material_settings.dst_factor));
    glBlendEquation(static_cast<int>(m_material_settings.blend_op));
}

MaterialSettings Material::materialSettings() const{
    return m_material_settings;
}