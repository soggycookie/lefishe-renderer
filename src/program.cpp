#include "program.h"
#include "shader.h"

using namespace Lefishe;

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
    LOG_TRACE("[PROGRAM] ID {0}: {1}", m_id, index);
    m_shader_objs[index++] = shader;
}

void Program::link() const {
    glLinkProgram(m_id);

    int success;
    char info_log[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);

    if (!success) {
       glGetProgramInfoLog(m_id, 512, NULL, info_log);
       LOG_WARN("[PROGRAM] Compilation failed! \n[INFO] {0}", info_log);
    }
}

void Program::bind() const {
    glUseProgram(m_id);
}
void Program::clear() {
    glDeleteProgram(m_id);
}

