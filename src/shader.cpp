#include "shader.h"

using namespace Lefishe;

Shader::Shader(STRING& src, PipelineStage type)
	: m_stage(type) 
{
	compile(src);
}

UINT Shader::id() const{
	return m_id;
}

STRING Shader::getStageStr() const {
    STRING type;

    switch (m_stage) {
    case PipelineStage::VERTEX:
        type = "VERTEX"; break;
    case PipelineStage::FRAGMENT:
        type = "FRAGMENT"; break;
    case PipelineStage::GEOMETRY:
        type = "GEOMETRY"; break;
    case PipelineStage::COMPUTE:
        type = "COMPUTE"; break;
    }

	return type;
}

void Shader::clear() {
	glDeleteShader(m_id);
    m_id = 0;
}

void Shader::compile(const STRING& src) {
    if (m_stage == PipelineStage::NONE) {
        LOG_WARN( "[SHADER] Pipeline stage is invalid");
        return;
    }

    m_id = glCreateShader(static_cast<UINT>(m_stage));
    const GLchar* c_src = src.c_str();
    glShaderSource(m_id, 1, &c_src, NULL);
    glCompileShader(m_id);
    

    int  success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

    std::string type;

    switch (m_stage) {
    case PipelineStage::VERTEX:
        type = "VERTEX"; break;
    case PipelineStage::FRAGMENT:
        type = "FRAGMENT"; break;
    case PipelineStage::GEOMETRY:
        type = "GEOMETRY"; break;
    case PipelineStage::COMPUTE:
        type = "COMPUTE"; break;
    }

    char info_log[512];
    if (!success)
    {
        glGetShaderInfoLog(m_id, 512, NULL, info_log);
        LOG_ERROR("[SHADER] {0} compilation failed! \n [INFO] {1}", getStageStr(), info_log);
    }else{
		LOG_TRACE("[SHADER] {0} compilation successfully!", getStageStr() );
    }
}

void ShaderLoader::parseShader(const STRING& path, ShaderSource& src_out) {
    std::fstream fstream(path);

    enum ShaderType {
        NONE = 0,
        VERT = 1,
        FRAG = 2,
        GEO = 3,
        COMPUTE = 4
    };

    ShaderType type = NONE;

    std::stringstream ss[4];

    if (fstream.is_open()) {
        std::string line;
        while (std::getline(fstream, line)) {
            if (line.find("#vertex") != std::string::npos) {
                type = VERT;
            }
            else if (line.find("#fragment") != std::string::npos) {
                type = FRAG;
            }
            else if (line.find("#geometry") != std::string::npos)
            {
                type = GEO;
            }
            else if (line.find("#compute") != std::string::npos)
            {
                type = COMPUTE;
            }
            else
            {
                if (type != NONE)
                {
                    if (!line.empty())
                    {
                        ss[static_cast<int>(type) - 1] << line << '\n';
                    }
                }
            }
        }

        src_out.vert = std::move(ss[0].str());
        src_out.frag = std::move(ss[1].str());
        src_out.geo = std::move(ss[2].str());
        src_out.compute = std::move(ss[3].str());

        fstream.close();
    }
    else {
        LOG_WARN("[SHADER] {0} can't open!", path);
    }
}