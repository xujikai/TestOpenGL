//
// Created by 徐极凯 on 2022/4/17.
//

#include "NV21TextureMapSample.h"
#include "../util/GLUtils.h"

static GLfloat mVertexArr[] = {
        -1.0f,  0.78f, 0.0f,  // Position 0
        -1.0f, -0.78f, 0.0f,  // Position 1
        1.0f,  -0.78f, 0.0f,  // Position 2
        1.0f,   0.78f, 0.0f,  // Position 3
};
static GLfloat mTexCoordArr[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};
static GLushort mIndexArr[] = {0, 1, 2, 0, 2, 3};

NV21TextureMapSample::NV21TextureMapSample() {
    aPositionLoc = GL_NONE;
    aTexCoordLoc = GL_NONE;
    uYTextureLoc = GL_NONE;
    uUvTextureLoc = GL_NONE;

    uYTexture = GL_NONE;
    uUvTexture = GL_NONE;
}

NV21TextureMapSample::~NV21TextureMapSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void NV21TextureMapSample::Init() {
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
            "uniform sampler2D uYTexture;                        \n"
            "uniform sampler2D uUvTexture;                        \n"
            "void main()                                         \n"
            "{                                                   \n"
            "	vec3 yuv;										\n"
            "   yuv.x = texture(uYTexture, vTexCoord).r;  	\n"
            "   yuv.y = texture(uUvTexture, vTexCoord).a-0.5;	\n"
            "   yuv.z = texture(uUvTexture, vTexCoord).r-0.5;	\n"
            "	highp vec3 rgb = mat3( 1,       1,       	1,					\n"
            "               0, 		-0.344, 	1.770,					\n"
            "               1.403,  -0.714,       0) * yuv; 			\n"
            "	fragColor = vec4(rgb, 1);						\n"
            "}                                                   \n";

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);

    aPositionLoc = 0;
    aTexCoordLoc = 1;
    uYTextureLoc = glGetUniformLocation(mProgramObj, "uYTexture");
    uUvTextureLoc = glGetUniformLocation(mProgramObj, "uUvTexture");

    GLuint textureIds[2] = {0};
    glGenTextures(2, textureIds);
    uYTexture = textureIds[0];
    uUvTexture = textureIds[1];

    glBindTexture(GL_TEXTURE_2D, uYTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glBindTexture(GL_TEXTURE_2D, uUvTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void NV21TextureMapSample::Draw(int screenW, int screenH) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(mProgramObj);

    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), mVertexArr);
    glEnableVertexAttribArray(aTexCoordLoc);
    glVertexAttribPointer(aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mTexCoordArr);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uYTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glUniform1i(uYTextureLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, uUvTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[1]);
    glUniform1i(uUvTextureLoc, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mIndexArr);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void NV21TextureMapSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &uYTexture);
        glDeleteTextures(1, &uUvTexture);
        mProgramObj = GL_NONE;
    }
}

void NV21TextureMapSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}
