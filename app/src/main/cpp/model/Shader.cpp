//
// Created by Administrator on 2022/8/4.
//

#include "Shader.h"

Shader::Shader(const char *vertShader, const char *fragShader) {
    program = GLUtils::CreateProgram(vertShader, fragShader);
}

void Shader::Use() {
    glUseProgram(program);
}

void Shader::Destroy() {
    GLUtils::DeleteProgram(program);
}