#include "program.h"
#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Lefishe;

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

Program::UniformInfo Program::getUniform(const std::string& u) const{
    return m_uniforms.at(u);
}

void Program::setUniform(const std::string& u, const MAT4& val) const{
    auto info = getUniform(u);
    glUniformMatrix4fv(info.location, 1, GL_FALSE, glm::value_ptr(val));
}

void Program::retrieveUniforms(){
    INT uniform_count = 0;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniform_count);

    if (uniform_count != 0)
    {
	    GLint 	max_name_len = 0;
	    GLsizei length = 0;
	    GLsizei size = 0;
	    GLenum 	type = GL_NONE;
	    glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);
	
	    auto uniform_name = std::make_unique<char[]>(max_name_len);



	    for (GLint i = 0; i < uniform_count; ++i)
	    {
		    glGetActiveUniform(m_id, i, max_name_len, &length, &size, &type, uniform_name.get());

		    UniformInfo uniform_info;
		    uniform_info.location = glGetUniformLocation(m_id, uniform_name.get());
		    uniform_info.size = size;

		    m_uniforms.emplace(std::make_pair(std::string(uniform_name.get(), length), uniform_info));
	    }
    }
}


std::shared_ptr<Program> ProgramFactory::createProgram(const std::string& path){
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

