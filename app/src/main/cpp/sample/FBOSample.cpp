//
// Created by 徐极凯 on 2022/4/17.
//

#include "FBOSample.h"
#include "../util/GLUtils.h"

//顶点坐标
static GLfloat mVertexArr[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
};
//正常纹理坐标
static GLfloat mTexCoordArr[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
};
//fbo 纹理坐标与正常纹理方向不同，原点位于左下角
static GLfloat mFboTexCoordArr[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
};
static GLushort mIndexArr[] = {0, 1, 2, 1, 3, 2};

FBOSample::FBOSample() {
    mFboProgramObj = GL_NONE;
    mFboId = GL_NONE;

    uTextureLoc = GL_NONE;
    uFboTextureLoc = GL_NONE;

    mVaoIds[0] = GL_NONE;
    mVboIds[0] = GL_NONE;

    mImageTexture = GL_NONE;
    mFboTexture = GL_NONE;
}

FBOSample::~FBOSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void FBOSample::Init() {
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
    // 用于普通渲染的片段着色器脚本，简单纹理映射
    char fShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 vTexCoord;\n"
            "layout(location = 0) out vec4 fragColor;\n"
            "uniform sampler2D uTexture;\n"
            "void main()\n"
            "{\n"
            "    fragColor = texture(uTexture, vTexCoord);\n"
            "}";
    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 vTexCoord;\n"
            "layout(location = 0) out vec4 fragColor;\n"
            "uniform sampler2D uTexture;\n"
            "void main()\n"
            "{\n"
            "    vec4 textureColor = texture(uTexture, vTexCoord);\n"
            "    float luminance = textureColor.r * 0.299 + textureColor.g * 0.587 + textureColor.b * 0.114;\n"
            "    fragColor = vec4(vec3(luminance), textureColor.a);\n"
            "}"; // 输出灰度图

    mProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);
    mFboProgramObj = GLUtils::CreateProgram(vShaderStr, fFboShaderStr);

    aPositionLoc = 0;
    aTexCoordLoc = 1;
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");

    aFboPositionLoc = 0;
    aFboTexCoordLoc = 1;
    uFboTextureLoc = glGetUniformLocation(mFboProgramObj, "uTexture");

    // 生成 VBO
    glGenBuffers(4, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoordArr), mTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mFboTexCoordArr), mFboTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    // 生成 VAO
    glGenVertexArrays(2, mVaoIds);

    glBindVertexArray(mVaoIds[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(aPositionLoc);
    glVertexAttribPointer(aPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(aTexCoordLoc);
    glVertexAttribPointer(aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBindVertexArray(GL_NONE);

    glBindVertexArray(mVaoIds[1]);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(aFboPositionLoc);
    glVertexAttribPointer(aFboPositionLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glEnableVertexAttribArray(aFboTexCoordLoc);
    glVertexAttribPointer(aFboTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBindVertexArray(GL_NONE);

    // 生成纹理
    glGenTextures(1, &mImageTexture);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    if (!CreateFrameBufferObj()) {
        LOGCATE("FBOSample::Init CreateFrameBufferObj fail");
    }
}

void FBOSample::Draw(int screenW, int screenH) {
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, mRenderImage.width, mRenderImage.height);

    glUseProgram(mFboProgramObj);

    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glBindVertexArray(mVaoIds[1]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glUniform1i(uFboTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // 通过上面的步骤，已经将数据绘制在 mFboTexture 上，可以读取里面的像素数据，也可以把该纹理显示在屏幕上
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, screenW, screenH);

    glUseProgram(mProgramObj);

    glBindVertexArray(mVaoIds[0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFboTexture);
    glUniform1i(uTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindVertexArray(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void FBOSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
    }
    if (mFboProgramObj) {
        glDeleteProgram(mFboProgramObj);
    }
    if (mImageTexture) {
        glDeleteTextures(1, &mImageTexture);
    }
    if (mFboTexture) {
        glDeleteTextures(1, &mFboTexture);
    }
    if (mVboIds[0]) {
        glDeleteBuffers(4, mVboIds);
    }
    if (mVaoIds[0]) {
        glDeleteVertexArrays(2, mVaoIds);
    }
    if (mFboId) {
        glDeleteFramebuffers(1, &mFboId);
    }
}

void FBOSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

bool FBOSample::CreateFrameBufferObj() {
    glGenTextures(1, &mFboTexture);
    glBindTexture(GL_TEXTURE_2D, mFboTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenFramebuffers(1, &mFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glBindTexture(GL_TEXTURE_2D, mFboTexture);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboTexture, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("FBOSample::CreateFrameBufferObj glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}
