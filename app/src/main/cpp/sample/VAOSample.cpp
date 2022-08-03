//
// Created by 徐极凯 on 2022/4/17.
//

#include "VAOSample.h"
#include "../util/GLUtils.h"

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_STRIDE         (sizeof(GLfloat)*(VERTEX_POS_SIZE+VERTEX_COLOR_SIZE))

// 4 mVertexAndColorArr, with(x,y,z) ,(r, g, b, a) per-vertex
static GLfloat mVertexAndColorArr[4 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE )] =
        {
                -0.5f,  0.5f, 0.0f,       // v0
                1.0f,  0.0f, 0.0f, 1.0f,  // c0
                -0.5f, -0.5f, 0.0f,       // v1
                0.0f,  1.0f, 0.0f, 1.0f,  // c1
                0.5f, -0.5f, 0.0f,        // v2
                0.0f,  0.0f, 1.0f, 1.0f,  // c2
                0.5f,  0.5f, 0.0f,        // v3
                0.5f,  1.0f, 1.0f, 1.0f,  // c3
        };
// Index buffer data
static GLushort mIndexArr[6] = {0, 1, 2, 0, 2, 3};

void VAOSample::Init() {
    const char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 aPosition;   \n"
            "layout(location = 1) in vec4 aColor;      \n"
            "out vec4 vColor;                          \n"
            "out vec4 vPosition;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "    vColor = aColor;                     \n"
            "    gl_Position = aPosition;              \n"
            "    vPosition = aPosition;               \n"
            "}";
    const char fShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec4 vColor;\n"
            "in vec4 vPosition;\n"
            "out vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "    float n = 10.0;\n"
            "    float span = 1.0 / n;\n"
            "    int i = int((vPosition.x + 0.5)/span);\n"
            "    int j = int((vPosition.y + 0.5)/span);\n"
            "\n"
            "    int grayColor = int(mod(float(i+j), 2.0));\n"
            "    if(grayColor == 1)\n"
            "    {\n"
            "        float luminance = vColor.r*0.299 + vColor.g*0.587 + vColor.b*0.114;\n"
            "        fragColor = vec4(vec3(luminance), vColor.a);\n"
            "    }\n"
            "    else\n"
            "    {\n"
            "        fragColor = vColor;\n"
            "    }\n"
            "}";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);

    aPositionLoc = 0;
    aColorLoc = 1;

    // 生成 VBO 并加载数据
    glGenBuffers(2, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexAndColorArr), mVertexAndColorArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    // 生成 VAO
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[1]);

    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE, 0);
    glEnableVertexAttribArray(aColorLoc);
    glVertexAttribPointer(aColorLoc, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, VERTEX_STRIDE,
                          (const void *)(VERTEX_POS_SIZE * sizeof(GLfloat)));

    glBindVertexArray(GL_NONE);
}

void VAOSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);

    glBindVertexArray(mVaoId);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(GL_NONE);
}

void VAOSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(2, mVboIds);
        glDeleteVertexArrays(1, &mVaoId);
        mProgramObj = GL_NONE;
    }
}
