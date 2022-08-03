//
// Created by 徐极凯 on 2022/4/17.
//

#include "TextureMapSample.h"
#include "../util/GLUtils.h"

static GLfloat mVertexArr[] = {
        -1.0f,  0.5f, 0.0f,  // Position 0
        -1.0f, -0.5f, 0.0f,  // Position 1
        1.0f, -0.5f, 0.0f,   // Position 2
        1.0f,  0.5f, 0.0f,   // Position 3
};
static GLfloat mTexCoordArr[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};
static GLushort mIndexArr[] = {0, 1, 2, 0, 2, 3};

TextureMapSample::TextureMapSample() {
    mImageTexture = GL_NONE;
}

TextureMapSample::~TextureMapSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void TextureMapSample::Init() {
    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 aPosition;   \n"
            "layout(location = 1) in vec2 aTexCoord;   \n"
            "out vec2 vTexCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = aPosition;               \n"
            "   vTexCoord = aTexCoord;                \n"
            "}                                          \n";
    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 vTexCoord;                                 \n"
            "layout(location = 0) out vec4 fragColor;             \n"
            "uniform sampler2D uTexture;                     \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  fragColor = texture(uTexture, vTexCoord);     \n"
            "}                                                   \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);

    aPositionLoc = 0;
    aTexCoordLoc = 1;
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");

    // 创建 RGBA 纹理
    glGenTextures(1, &mImageTexture);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void TextureMapSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);

    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), mVertexArr);
    glEnableVertexAttribArray(aTexCoordLoc);
    glVertexAttribPointer(aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mTexCoordArr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glUniform1i(uTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mIndexArr);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glUseProgram(GL_NONE);
}

void TextureMapSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &mImageTexture);
        mProgramObj = GL_NONE;
    }
}

void TextureMapSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}
