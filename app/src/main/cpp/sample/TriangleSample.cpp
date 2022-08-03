//
// Created by 徐极凯 on 2022/4/16.
//

#include "TriangleSample.h"
#include "../util/GLUtils.h"
#include "../util/LogUtil.h"

static GLfloat mVertexArr[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
};

TriangleSample::TriangleSample() {
    aPositionLocation = 0;
}

TriangleSample::~TriangleSample() = default;

void TriangleSample::Init() {
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 aPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = aPosition;              \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4(1.0, 0.0, 0.0, 1.0);     \n"
            "}                                            \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);
}

void TriangleSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);

    glVertexAttribPointer(aPositionLocation, 3, GL_FLOAT, GL_FALSE, 0, mVertexArr);
    glEnableVertexAttribArray(aPositionLocation);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(GL_NONE);
}

void TriangleSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        mProgramObj = GL_NONE;
    }
}
