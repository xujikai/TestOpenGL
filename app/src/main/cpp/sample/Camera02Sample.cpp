//
// Created by Administrator on 2023/1/4.
//

#include "Camera02Sample.h"

static GLfloat mVertexArr[] = {
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
};

static GLfloat mTexCoordArr[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};

const static std::string vShaderStr = R"(
    #version 300 es
    layout(location = 0) in vec4 aPosition;
    layout(location = 1) in vec2 aTexCoord;
    uniform mat4 uMvpMatrix;
    out vec2 vTexCoord;

    void main() {
        gl_Position = uMvpMatrix * aPosition;
        vTexCoord = aTexCoord;
    }
)";

const static std::string fShaderStr = R"(
    #version 300 es
    precision mediump float;
    in vec2 vTexCoord;
    layout(location = 0) out vec4 outColor;
    uniform sampler2D uTextureY;
    uniform sampler2D uTextureU;
    uniform sampler2D uTextureV;

    void main() {
        float y, u, v, r, g, b;
        y = texture(uTextureY, vTexCoord).r;
        u = texture(uTextureU, vTexCoord).r;
        v = texture(uTextureV, vTexCoord).r;
        u = u - 0.5;
        v = v - 0.5;
        r = y + 1.403 * v;
        g = y - 0.344 * u - 0.714 * v;
        b = y + 1.770 * u;
        outColor = vec4(r, g, b, 1.0);
    }
)";

Camera02Sample::~Camera02Sample() {
    LOGCATE("~Camera02Sample start");
    NativeImageUtil::FreeNativeImage(&mRenderImage);
    LOGCATE("~Camera02Sample end");
}

int Camera02Sample::Init() {
    return 0;
}

int Camera02Sample::UnInit() {
    LOGCATE("unInit start");
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &mImageTexYId);
        glDeleteTextures(1, &mImageTexUId);
        glDeleteTextures(1, &mImageTexVId);
        mProgramObj = GL_NONE;
    }
    LOGCATE("unInit end");
    return 0;
}

void Camera02Sample::OnSurfaceCreated() {
    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), fShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureYLoc = glGetUniformLocation(mProgramObj, "uTextureY");
    uTextureULoc = glGetUniformLocation(mProgramObj, "uTextureU");
    uTextureVLoc = glGetUniformLocation(mProgramObj, "uTextureV");

    glGenTextures(1, &mImageTexYId);
    glBindTexture(GL_TEXTURE_2D, mImageTexYId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenTextures(1, &mImageTexUId);
    glBindTexture(GL_TEXTURE_2D, mImageTexUId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenTextures(1, &mImageTexVId);
    glBindTexture(GL_TEXTURE_2D, mImageTexVId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Camera02Sample::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    m_SurfaceWidth = width;
    m_SurfaceHeight = height;
}

void Camera02Sample::OnDrawFrame() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (mRenderImage.ppPlane[0] == nullptr) return;

    ByteFlowLock lock(&mSyncLock);
    if (isShaderChanged) {
        UpdateProgram();
        isShaderChanged = false;
    }

    glUseProgram(mProgramObj);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mVertexArr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mTexCoordArr);

    UpdateMVPMatrix(mMvpMatrix, mTransformMatrix);
    glUniformMatrix4fv(uMvpMatrixLoc, 1, GL_FALSE, &mMvpMatrix[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageTexYId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mRenderImage.width, mRenderImage.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[0]);
    glUniform1i(uTextureYLoc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mImageTexUId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mRenderImage.width >> 1, mRenderImage.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[1]);
    glUniform1i(uTextureULoc, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mImageTexVId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mRenderImage.width >> 1, mRenderImage.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, mRenderImage.ppPlane[2]);
    glUniform1i(uTextureVLoc, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void Camera02Sample::UpdateFrame(NativeImage *pImage) {
    if (pImage) {
        mRenderImage.width = pImage->width;
        mRenderImage.height = pImage->height;
        mRenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &mRenderImage);
    }
}

void Camera02Sample::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY,
                                   int degree, int mirror) {
    mTransformMatrix.degree = degree;
    mTransformMatrix.mirror = mirror;
}

void Camera02Sample::LoadShaderStr(int shaderIndex, char *pShaderStr, int strLen) {
//    if (!mFragShaderStr.empty()) {
//        delete &mFragShaderStr;
//    }
    ByteFlowLock lock(&mSyncLock);
    mFragShaderStr = std::string(pShaderStr);
    isShaderChanged = true;
}

void Camera02Sample::UpdateProgram() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        mProgramObj = GL_NONE;
    }

    LOGCATE("shader str: %s", mFragShaderStr.data());
    mProgramObj = GLUtils::CreateProgram(vShaderStr.data(), mFragShaderStr.data());

    uMvpMatrixLoc = glGetUniformLocation(mProgramObj, "uMvpMatrix");
    uTextureYLoc = glGetUniformLocation(mProgramObj, "uTextureY");
    uTextureULoc = glGetUniformLocation(mProgramObj, "uTextureU");
    uTextureVLoc = glGetUniformLocation(mProgramObj, "uTextureV");
}

void Camera02Sample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix) {
    float fFactorX = 1.0f;
    float fFactorY = 1.0f;

    if (transformMatrix.mirror == 1) {
        fFactorX = -1.0f;
    } else if (transformMatrix.mirror == 2) {
        fFactorY = -1.0f;
    }

    float fRotate = MATH_PI * transformMatrix.degree * 1.0f / 180;
    if (transformMatrix.mirror == 0) {
        if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 1.5;
        }
    } else if (transformMatrix.mirror == 1) {
        if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 1.5;
        }
    }

    // float ratio = (float) mRenderImage.width / mRenderImage.height;
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    // glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 4.0f, 100.0f);
    // glm::mat4 Projection = glm::perspective(45.0f,4.0f/3.0f,0.1f,100.f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(fFactorX * transformMatrix.scaleX,
                                        fFactorY * transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));

//    LOGCATE("SetMVP real: rotate %d,%.2f,%0.5f,%0.5f,%0.5f,%0.5f,", transformMatrix.degree, fRotate,
//            transformMatrix.translateX, transformMatrix.translateY,
//            fFactorX * transformMatrix.scaleX, fFactorY * transformMatrix.scaleY);

    mvpMatrix = Projection * View * Model;
}
