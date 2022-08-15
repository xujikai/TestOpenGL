//
// Created by Administrator on 2022/8/5.
//

#include "PBOSample.h"

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

static GLushort mIndexArr[] = { 0, 1, 2, 1, 3, 2 };

void PBOSample::Init() {
    char vShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 aPosition;   \n"
            "layout(location = 1) in vec2 aTexCoord;   \n"
            "uniform mat4 uMvpMatrix;                  \n"
            "out vec2 vTexCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = uMvpMatrix * aPosition;   \n"
            "   vTexCoord = aTexCoord;                  \n"
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

    // 用于离屏渲染的顶点着色器脚本，不使用变换矩阵
    char vFboShaderStr[] =
            "#version 300 es                            \n"
            "layout(location = 0) in vec4 aPosition;   \n"
            "layout(location = 1) in vec2 aTexCoord;   \n"
            "out vec2 vTexCoord;                       \n"
            "void main()                                \n"
            "{                                          \n"
            "   gl_Position = aPosition;               \n"
            "   vTexCoord = aTexCoord;                \n"
            "}                                          \n";

    // 用于离屏渲染的片段着色器脚本，取每个像素的灰度值
    char fFboShaderStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "in vec2 vTexCoord;\n"
            "layout(location = 0) out vec4 fragColor;\n"
            "uniform sampler2D uTexture;\n"
            "void main()\n"
            "{\n"
            "    vec4 tempColor = texture(uTexture, vTexCoord);\n"
            "    float luminance = tempColor.r * 0.299 + tempColor.g * 0.587 + tempColor.b * 0.114;\n"
            "    fragColor = vec4(vec3(luminance), tempColor.a);\n"
            "}"; // 输出灰度图

    mProgram = GLUtils::CreateProgram(vShaderStr, fShaderStr);
    mFboProgram = GLUtils::CreateProgram(vFboShaderStr, fFboShaderStr);

    // 获取 shader 中位置值
    uMvpMatrixLoc = glGetUniformLocation(mProgram, "uMvpMatrix");
    uTextureLoc = glGetUniformLocation(mProgram, "uTexture");
    uFboTextureLoc = glGetUniformLocation(mFboProgram, "uTexture");

    // 定义 vbo 和 ebo
    glGenBuffers(4, mVboIds);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertexArr), mVertexArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mTexCoordArr), mTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mFboTexCoordArr), mFboTexCoordArr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndexArr), mIndexArr, GL_STATIC_DRAW);

    glGenVertexArrays(2, mVaoIds);
    glBindVertexArray(mVaoIds[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBindVertexArray(GL_NONE);

    glBindVertexArray(mVaoIds[1]);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[2]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[3]);
    glBindVertexArray(GL_NONE);

    // 加载纹理数据
    glGenTextures(1, &mImageTexture);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    // 创建 pbo
    glGenBuffers(2, mPboUploadIds);
    int imageByteSize = mRenderImage.width * mRenderImage.height * 4;
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboUploadIds[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, imageByteSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboUploadIds[1]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, imageByteSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, GL_NONE);

    glGenBuffers(2, mPboDownloadIds);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboDownloadIds[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, imageByteSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboDownloadIds[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, imageByteSize, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, GL_NONE);

    if (!CreateFrameBufferObj()) {
        LOGCATE("PBOSample::Init CreateFrameBufferObj failed");
        return;
    }
}

void PBOSample::Draw(int screenW, int screenH) {
    UploadPixels();

    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glViewport(0, 0, mRenderImage.width, mRenderImage.height);
    glUseProgram(mFboProgram);
    glBindVertexArray(mVaoIds[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexture);
    glUniform1i(uFboTextureLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);

    DownloadPixels();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, screenW, screenH);
    glUseProgram(mProgram);
    glBindVertexArray(mVaoIds[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFboImageTexture);
    glUniform1i(uTextureLoc, 0);
    UpdateMVPMatrix(mMvpMatrix, mRotateX, mRotateY, (float) screenW / (float) screenH);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindVertexArray(GL_NONE);

//    mRotateY += 1;
    mFrameIndex++;
}

void PBOSample::UnInit() {
    if (mProgram) {
        glDeleteProgram(mProgram);
        mProgram = GL_NONE;
    }
    if (mFboProgram) {
        glDeleteProgram(mFboProgram);
        mFboProgram = GL_NONE;
    }
    if (mImageTexture) {
        glDeleteTextures(1, &mImageTexture);
    }
    if (mFboImageTexture) {
        glDeleteTextures(1, &mFboImageTexture);
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
    if (mPboUploadIds[0]) {
        glDeleteBuffers(2, mPboUploadIds);
    }
    if (mPboDownloadIds[0]) {
        glDeleteBuffers(2, mPboDownloadIds);
    }
}

void PBOSample::SetImageData(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

void PBOSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio) {
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

bool PBOSample::CreateFrameBufferObj() {
    glGenTextures(1, &mFboImageTexture);
    glBindTexture(GL_TEXTURE_2D, mFboImageTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &mFboId);
    glBindFramebuffer(GL_FRAMEBUFFER, mFboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFboImageTexture, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mRenderImage.width, mRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGCATE("PBOSample::CreateFrameBufferObj status is not GL_FRAMEBUFFER_COMPLETE");
        return false;
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}

void PBOSample::UploadPixels() {
    int index = mFrameIndex % 2;
    int nextIndex = (index + 1) % 2;
    int imageByteSize = mRenderImage.width * mRenderImage.height * 4;

    BEGIN_TIME("PBOSample::UploadPixels Copy Pixels from PBO to Textrure Obj")
        glBindTexture(GL_TEXTURE_2D, mFboImageTexture);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboUploadIds[index]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mRenderImage.width, mRenderImage.height,
                        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    END_TIME("PBOSample::UploadPixels Copy Pixels from PBO to Textrure Obj")

    BEGIN_TIME("PBOSample::UploadPixels Update Image data")
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboUploadIds[nextIndex]);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, imageByteSize, nullptr, GL_STREAM_DRAW);
        GLubyte *pBuf = (GLubyte *) glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, imageByteSize,
                                                     GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        if (pBuf) {
            // 将 mRenderImage 的数据复制到 PBO 中
            memcpy(pBuf, mRenderImage.ppPlane[0], imageByteSize);
            // 随机修改图像的像素数据，只是为了测试
            int randomRow = rand() % (mRenderImage.height - 5);
            memset(pBuf + randomRow * mRenderImage.width * 4, 188, mRenderImage.width * 4 * 5);
            glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        }
    END_TIME("PBOSample::UploadPixels Update Image data")
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, GL_NONE);
}

void PBOSample::DownloadPixels() {
    int index = mFrameIndex % 2;
    int nextIndex = (index + 1) % 2;
    int imageByteSize = mRenderImage.width * mRenderImage.height * 4;

//    NativeImage nativeImage;
//    nativeImage.format = IMAGE_FORMAT_RGBA;
//    uint8_t *pImageBuf = new uint8_t[imageByteSize];
//    nativeImage.ppPlane[0] = pImageBuf;
//    BEGIN_TIME("DownloadPixels glReadPixels without PBO")
//        glReadPixels(0, 0, mRenderImage.width, mRenderImage.height, GL_RGBA, GL_UNSIGNED_BYTE, pImageBuf);
//    END_TIME("DownloadPixels glReadPixels without PBO")
//    delete []pImageBuf;

    BEGIN_TIME("DownloadPixels glReadPixels with PBO")
        glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboDownloadIds[index]);
        glReadPixels(0, 0, mRenderImage.width, mRenderImage.height, GL_RGBA, GL_UNSIGNED_BYTE,
                     nullptr);
    END_TIME("DownloadPixels glReadPixels with PBO")

    BEGIN_TIME("DownloadPixels PBO glMapBufferRange")
        glBindBuffer(GL_PIXEL_PACK_BUFFER, mPboDownloadIds[nextIndex]);
        GLubyte *pBuf = (GLubyte *) glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, imageByteSize, GL_MAP_READ_BIT);
        if (pBuf) {
            NativeImage nativeImage;
            nativeImage.format = IMAGE_FORMAT_RGBA;
            nativeImage.ppPlane[0] = pBuf;
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        }
    END_TIME("DownloadPixels PBO glMapBufferRange")
    glBindBuffer(GL_PIXEL_PACK_BUFFER, GL_NONE);
}
