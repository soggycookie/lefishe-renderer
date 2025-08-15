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

        for(const auto& [key, textures] : m_textures){
            for(int i =0 ;i < textures.size() ;i++){
                if(auto tex = textures[i].lock()){
                    tex->bind(getTextureSlot(key, i + 1));
                }
            }
        }

	    for(const auto& [key, value] : m_uniform_data){
		    p->setUniform(key, value);
	    }
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

INT Material::getTextureSlot(const STRING& name, INT count) const{
    if(!m_texture_slots.contains(name)){
        return -1;
    }
    
    auto p = program();
    auto pair = p->getUniformInfo(name);

    if(count > pair.count){
        LOG_ERROR("{0} has size of {1}, and be assigned with {2}", name, pair.count, count);
        return -1;
    }

    INT* slots = m_texture_slots.at(name);
    LOG_INFO("bind slot: {0}", slots[count - 1]);
    return slots[count - 1];
}

void Material::assignTexture(const STRING& name, std::shared_ptr<Texture> texture){
    if(!m_textures.contains(name)){
        return;
    }

    auto p = program();
    auto pair = p->getUniformInfo(name);

    if(m_textures[name].size() >= pair.count){
        return;
    }

    m_textures[name].push_back(texture);
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
            bind_slots = new INT[size];
			p->getUniformValue(key, bind_slots);
            m_texture_slots[key] = bind_slots;
            m_textures[key] = {};
        }



            break;
        }
	}
}



void Material::bindSettings() const{
    //if(m_material_settings.enable_depth){
    //    glEnable(GL_DEPTH_TEST);
    //    glDepthMask(m_material_settings.depth_written ? GL_TRUE : GL_FALSE);
    //    glDepthFunc(static_cast<INT>(m_material_settings.depth_func));
    //}else{
    //    glDisable(GL_DEPTH_TEST);
    //}

    if(m_material_settings.is_cull){
        glEnable(GL_CULL_FACE);
        glCullFace(static_cast<INT>(m_material_settings.cull_mode));
    }else{
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, static_cast<INT>(m_material_settings.polygon_mode));

}

MaterialSettings Material::materialSettings() const{
    return m_material_settings;
}