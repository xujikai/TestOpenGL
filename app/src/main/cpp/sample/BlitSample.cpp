//
// Created by 徐极凯 on 2022/8/13.
//

#include "BlitSample.h"

static GLfloat mVertexArr[] = {
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
};

static GLfloat mTexCoordArr[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
};

static GLushort mIndexArr[] = {0, 1, 2, 0, 2, 3};

const GLenum mFboTexAttachments[4] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3
};

BlitSample::~BlitSample() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
}

void BlitSample::Init() {
    static std::string vShaderStr = R"(
        #version 300 es
        layout(location = 0) in vec4 aPosition;
        layout(location = 1) in vec2 aTextureCoord;
        uniform mat4 uMvpMatrix;
        out vec2 vTextureCoord;
        void main() {
            gl_Position = uMvpMatrix * aPosition;
            vTextureCoord = aTextureCoord;
        }
    )";
    static std::string fShaderStr = R"(
        #version 300 es
        precision mediump float;
        layout(location = 0) out vec4 fragColor0;
        layout(location = 1) out vec4 fragColor1;
        layout(location = 2) out vec4 fragColor2;
        layout(location = 3) out vec4 fragColor3;
        in vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main() {
            vec4 textureColor = texture(uTexture, vTextureCoord);
            fragColor0 = textureColor;
            fragColor1 = vec4(textureColor.r, 0.0, 0.0, 1.0);
            fragColor2 = vec4(0.0, textureColor.g, 0.0, 1.0);
            fragColor3 = vec4(0.0, 0.0, textureColor.b, 1.0);
        }
    )";

    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureLoc = glGetUniformLocation(mProgramObj, "uTexture");

    glGenBuffers(3, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoordArr), mTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);

    glBindVertexArray(GL_NONE);

    glGenTextures(1, &mImageTexId);
    glBindTexture(GL_TEXTURE_2D, mImageTexId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    CreateFrameBufferObj();
}

void BlitSample::Draw(int screenW, int screenH) {
    mSurfaceWidth = screenW;
    mSurfaceHeight = screenH;

    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glViewport(0, 0, mRenderImage.width, mRenderImage.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glDrawBuffers(4, mFboTexAttachments);

    glUseProgram(mProgramObj);
    glBindVertexArray(mVaoId);

    UpdateMVPMatrix(mMvpMatrix, 0, mRotateY, (float)screenW / (float)screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    for (int i = 0; i < 4; ++i) {
        glBindTexture(GL_TEXTURE_2D, mFboImageTexIds[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glUniform1i(uTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    // 指定目标帧缓冲区
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    glViewport(0, 0, screenW, screenH);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 指定源帧缓冲区
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFboId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, mRenderImage.width, mRenderImage.height,
                      0, 0, mSurfaceWidth / 2, mSurfaceHeight / 2,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, mRenderImage.width, mRenderImage.height,
                      mSurfaceWidth / 2, 0, mSurfaceWidth, mSurfaceHeight / 2,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, mRenderImage.width, mRenderImage.height,
                      0, mSurfaceHeight / 2, mSurfaceWidth / 2, mSurfaceHeight,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glReadBuffer(GL_COLOR_ATTACHMENT3);
    glBlitFramebuffer(0, 0, mRenderImage.width, mRenderImage.height,
                      mSurfaceWidth / 2, mSurfaceHeight / 2, mSurfaceWidth, mSurfaceHeight,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void BlitSample::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
    }
    if (mVboIds[0]) {
        glDeleteBuffers(3, mVboIds);
    }
    if (mVaoId) {
        glDeleteVertexArrays(1, &mVaoId);
    }
    if (mFboId) {
        glDeleteFramebuffers(1, &mFboId);
    }
    if (mImageTexId) {
        glDeleteTextures(1, &mImageTexId);
    }
    if (mFboImageTexIds[0]) {
        glDeleteTextures(4, mFboImageTexIds);
    }
}

void BlitSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

void BlitSample::CreateFrameBufferObj() {
    GLint defaultFboId = GL_NONE;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFboId);

    glGenFramebuffers(1, &mFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);

    glGenTextures(4, mFboImageTexIds);
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, mFboImageTexIds[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, mFboTexAttachments[i],
                               GL_TEXTURE_2D, mFboImageTexIds[i], 0);
    }
    // 告诉OpenGL要渲染到4个颜色附着上
    glDrawBuffers(4, mFboTexAttachments);

    glBindFramebuffer(GL_FRAMEBUFFER, defaultFboId);
}

void BlitSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
    angleX = angleX % 360;
    angleY = angleY % 360;

    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 10.0f);

    mMvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}
