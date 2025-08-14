#include "program.h"
#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Lefishe;

Program::UniformInfo::UniformInfo(GLint loc, GLint c, GLenum t)
    : location(loc), count(c), type(t)
{
}

Program::Program(){
    create();
}

void Program::create() {
    m_id = glCreateProgram();
}

void Program::attach(const Shader& shader) {
    static int index = 0;
    if (m_id <= 0) {
        LOG_WARN("[PROGRAM] Shader attached to an uninitialized program!");
        return;
    }

    glAttachShader(m_id, shader.id());
    m_shader_objs[index++] = shader;
}

void Program::link(){
    if(m_id <= 0){
        LOG_WARN("[PROGRAM] Can't link to an invalid ID!");
        return;
    }

    glLinkProgram(m_id);

    int success;
    char info_log[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success) {
       glGetProgramInfoLog(m_id, 512, NULL, info_log);
       LOG_WARN("[PROGRAM] Compilation failed! \n[INFO] {0}", info_log);
    }

    retrieveUniforms();
}

void Program::bind() const {
    if(m_id <= 0){
        LOG_WARN("[PROGRAM]  Can't bind to an invalid ID!");
        return;
    }
    
    glUseProgram(m_id);
}

void Program::clear() {
    glDeleteProgram(m_id);
    m_id = 0;
}

Program::UniformInfo Program::getUniform(const STRING& u) const{
    return m_uniforms.at(u);
}


void Program::setUniform(const STRING& u, const void* data) const{
    auto info = getUniform(u);

    switch(info.type){
        case GL_FLOAT:
            glUniform1fv(info.location, info.count, static_cast<const FLOAT*>(data));
            break;
        case GL_FLOAT_VEC2:
            glUniform2fv(info.location, info.count, static_cast<const FLOAT*>(data));
            break;
        case GL_FLOAT_VEC3:
            glUniform3fv(info.location, info.count, static_cast<const FLOAT*>(data));
            break;
        case GL_FLOAT_VEC4:
            glUniform4fv(info.location, info.count, static_cast<const FLOAT*>(data));
            break;
        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(info.location, info.count, GL_FALSE, static_cast<const FLOAT*>(data));
            break;
        case GL_INT:
            glUniform1iv(info.location, info.count, static_cast<const INT*>(data));
            break;
        case GL_SAMPLER_2D:
            glUniform1iv(info.location, info.count, static_cast<const INT*>(data));
            break;
    }
}

void Program::retrieveUniforms(){
    INT uniform_count = 0;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0)
    {
	    GLint 	max_name_len = 0;
	    GLsizei length = 0;
	    GLsizei count = 0;
	    GLenum 	type = GL_NONE;
	    glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
	
	    auto uniform_name = std::make_unique<char[]>(max_name_len);


        //LOG_INFO("{0}", glGetUniformLocation(m_id, "here"));

        INT block_index = 0;
	    for (UINT i = 0; i < uniform_count; ++i)
	    {
		    glGetActiveUniform(m_id, i, max_name_len, &length, &count, &type, uniform_name.get());
            STRING name = STRING(uniform_name.get(), length);
            if(name.ends_with("[0]")){
                name = name.substr(0, name.size() - 3);
            }

            glGetActiveUniformsiv(m_id, 1,  &i, GL_UNIFORM_BLOCK_INDEX, &block_index);

            if(block_index == -1){
		        GLint location = glGetUniformLocation(m_id, uniform_name.get());
		        UniformInfo uniform_info(location, count , type);

		        m_uniforms.emplace(std::make_pair(name, uniform_info));
            }
	    }
    }
}

const std::unordered_map<STRING, Program::UniformInfo>& Program::uniform() const{
    return m_uniforms;
}


void Program::getUniformValue(GLint location, GLint size, FLOAT* out_data){
    glGetnUniformfv(m_id, location, size, out_data);
}

void Program::getUniformValue(GLint location, GLint size, INT* out_data){
    glGetnUniformiv(m_id, location, size, out_data);
}

void Program::getUniformValue(GLint location, GLint size, DOUBLE* out_data){
    glGetnUniformdv(m_id, location, size, out_data);
}

void Program::getUniformValue(GLint location, GLint size, UINT* out_data){
    glGetnUniformuiv(m_id, location, size, out_data);
}

void Program::getUniformValue(const STRING& name, void* out_data){
    auto info = m_uniforms[name];

	switch(info.type) {
	    case GL_FLOAT:
	    {
		    FLOAT* data = static_cast<FLOAT*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(FLOAT), &data[i]);
		    }
	    }
	    break;
	    case GL_FLOAT_VEC2:
	    {
		    VEC2* data = static_cast<VEC2*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(VEC2), glm::value_ptr(data[i]));
		    }
	    }
	    break;
	    case GL_FLOAT_VEC3:
	    {
		    VEC3* data = static_cast<VEC3*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(VEC3), glm::value_ptr(data[i]));
		    }
	    }
	    break;
	    case GL_FLOAT_VEC4:
	    {
		    VEC4* data = static_cast<VEC4*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(VEC4), glm::value_ptr(data[i]));
		    }
	    }
	    break;
	    case GL_FLOAT_MAT4:
	    {
		    MAT4* data = static_cast<MAT4*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(MAT4), glm::value_ptr(data[i]));
		    }
	    }
	    break;
	    case GL_INT :
	    {
		    INT* data = static_cast<INT*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(INT), &data[i]);
		    }
	    }
	    break;
	    case GL_SAMPLER_2D :
	    {
		    INT* data = static_cast<INT*>(out_data);
		    for(int i = 0; i < info.count; i++) {
			    getUniformValue(info.location + i, sizeof(INT), &data[i]);
		    }
	    }
	    break;
	}
}

std::shared_ptr<Program> ProgramFactory::createProgram(const STRING& path){
    auto program = std::make_shared<Program>();

    ShaderSource ssrc;

    ShaderLoader::parseShader(path, ssrc);

    
        if (!ssrc.vert.empty()) {
            Shader vertex
            (ssrc.vert, PipelineStage::VERTEX);
            program->attach(vertex);
        }

        if (!ssrc.frag.empty()) {
            Shader fragment
            (ssrc.frag,PipelineStage::FRAGMENT);
            program->attach(fragment);
        }

        if (!ssrc.geo.empty()) {
            Shader geometry
            (ssrc.geo, PipelineStage::GEOMETRY);
            program->attach(geometry);
        }

        if (!ssrc.compute.empty()) {
            Shader compute
            (ssrc.compute, PipelineStage::COMPUTE);
            program->attach(compute);
        }

        program->link();

    return program;
}

