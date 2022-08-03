//
// Created by 徐极凯 on 2022/5/3.
//

#include <gtc/matrix_transform.hpp>
#include "CoordSystemSample.h"

static GLfloat mVertexArr[] = {
        -1.0f,  1.0f, 0.0f,  // Position 0
        -1.0f, -1.0f, 0.0f,  // Position 1
        1.0f,  -1.0f, 0.0f,  // Position 2
        1.0f,   1.0f, 0.0f,  // Position 3
};

static GLfloat mTexCoordArr[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};

static GLushort mIndexArr[] = {0, 1, 2, 0, 2, 3};

CoordSystemSample::CoordSystemSample() {
    mVaoId = GL_NONE;
    uTextureLoc = GL_NONE;
    uMVPMatrixLoc = GL_NONE;
    mImageTexture = GL_NONE;

    mRotateX = 0;
    mRotateY = 10;
    mScaleX = 1.0f;
    mScaleY = 1.0f;
}

CoordSystemSample::~CoordSystemSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void CoordSystemSample::Init() {
    if (mProgramObj != GL_NONE) return;

    char vShaderStr[] =
            "#version 300 es\n"
            "layout(location = 0) in vec4 aPosition;\n"
            "layout(location = 1) in vec2 aTexCoord;\n"
            "uniform mat4 uMVPMatrix;\n"
            "out vec2 vTexCoord;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = uMVPMatrix * aPosition;\n"
            "    vTexCoord = aTexCoord;\n"
            "}";

    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 vTexCoord;                                  \n"
            "layout(location = 0) out vec4 fragColor;            \n"
            "uniform sampler2D uTexture;                         \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  fragColor = texture(uTexture, vTexCoord);         \n"
            "}                                                   \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);

    aPositionLoc = 0;
    aTexCoordLoc = 1;
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");
    uMVPMatrixLoc = glGetUniformLocation(mProgramObj, "uMVPMatrix");

    // 生成 VBO 并绑定数据
    glGenBuffers(3, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoordArr), mTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    // 生成 VAO 并绑定数据
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
    glEnableVertexAttribArray(aPositionLoc);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glVertexAttribPointer(aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glEnableVertexAttribArray(aTexCoordLoc);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBindVertexArray(GL_NONE);

    // 生成纹理
    glGenTextures(1, &mImageTexture);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 给纹理赋值图像数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void CoordSystemSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);

    glBindVertexArray(mVaoId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glUniform1i(uTextureLoc, 0);

    UpdateMVPMatrix(mMVPMatrix, mRotateX, mRotateY, (float)screenW / (float)screenH);
    glUniformMatrix4fv(uMVPMatrixLoc, 1, GL_FALSE, &mMVPMatrix[0][0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

    glBindVertexArray(GL_NONE);
}

void CoordSystemSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteBuffers(3, mVboIds);
        glDeleteVertexArrays(1, &mVaoId);
        glDeleteTextures(1, &mImageTexture);
    }
}

void CoordSystemSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

void CoordSystemSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    mRotateX = static_cast<int>(rotateX);
    mRotateY = static_cast<int>(rotateY);

    mScaleX = scaleX;
    mScaleY = scaleY;
}

void CoordSystemSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio) {
    rotateX = rotateX % 360;
    rotateY = rotateY % 360;

    float radiansX = static_cast<float>(MATH_PI / 180 * rotateX);
    float radiansY = static_cast<float>(MATH_PI / 180 * rotateY);

    // 模型矩阵
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(mScaleX, mScaleY, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));

    // 视图矩阵
    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 0, 4),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    // 投影矩阵
    glm::mat4 projectionMatrix = glm::perspective(45.0f, ratio, 0.1f, 100.0f);

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}
