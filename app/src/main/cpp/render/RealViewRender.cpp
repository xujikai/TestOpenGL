//
// Created by Administrator on 2023/1/6.
//

#include "RealViewRender.h"

RealViewRender *RealViewRender::pInstance = nullptr;

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

static GLfloat mObjVertexArr[] = {
        -0.5f,  0.5f, 0.0f,  // Position 0
        -0.5f, -0.5f, 0.0f,  // Position 1
        0.5f, -0.5f, 0.0f,   // Position 2
        0.5f,  0.5f, 0.0f,   // Position 3
};

static GLfloat mObjTexCoordArr[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};

static GLushort mObjIndexArr[] = {0, 1, 2, 0, 2, 3};

RealViewRender::RealViewRender() {

}

RealViewRender::~RealViewRender() {
    NativeImageUtil::FreeNativeImage(&mRenderImage);
    NativeImageUtil::FreeNativeImage(&mObjRenderImage);
}

void RealViewRender::Init() {

}

void RealViewRender::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &mImageTexYId);
        glDeleteTextures(1, &mImageTexUId);
        glDeleteTextures(1, &mImageTexVId);
        mProgramObj = GL_NONE;
    }
    if (mObjProgramObj) {
        glDeleteProgram(mObjProgramObj);
        glDeleteTextures(1, &mObjImageTexId);
        mObjProgramObj = GL_NONE;
    }
    DestroyInstance();
}

void RealViewRender::OnSurfaceCreated() {
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

    mObjProgramObj = GLUtils::CreateProgram(vObjShaderStr, fObjShaderStr);
    uObjTextureLoc = glGetUniformLocation(mObjProgramObj, "uTexture");
    uObjMvpMatrixLoc = glGetUniformLocation(mObjProgramObj, "uMvpMatrix");

    glGenTextures(1, &mObjImageTexId);
    glBindTexture(GL_TEXTURE_2D, mObjImageTexId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void RealViewRender::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    mSurfaceWidth = width;
    mSurfaceHeight = height;
}

void RealViewRender::OnDrawFrame() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (mRenderImage.ppPlane[0] == nullptr) return;

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

//    if (mObjRenderImage.ppPlane[0] == nullptr) return;

    glUseProgram(mObjProgramObj);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), mObjVertexArr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), mObjTexCoordArr);

    UpdateObjMVPMatrix(mObjMvpMatrix, mTransformMatrix);
    glUniformMatrix4fv(uObjMvpMatrixLoc, 1, GL_FALSE, &mObjMvpMatrix[0][0]);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mObjImageTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mObjRenderImage.width, mObjRenderImage.height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, mObjRenderImage.ppPlane[0]);
    glUniform1i(uObjTextureLoc, 3);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, mObjIndexArr);

//    glDisable(GL_BLEND);
//    glDisable(GL_DEPTH_TEST);
}

void RealViewRender::SetImageData(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.width = mRenderImage.width = width;
    nativeImage.height = mRenderImage.height = height;
    nativeImage.format = mRenderImage.format = format;
    nativeImage.ppPlane[0] = pData;

    switch (format) {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
    NativeImageUtil::CopyNativeImage(&nativeImage, &mRenderImage);
}

void RealViewRender::SetObjImageData(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.width = mObjRenderImage.width = width;
    nativeImage.height = mObjRenderImage.height = height;
    nativeImage.format = mObjRenderImage.format = format;
    nativeImage.ppPlane[0] = pData;

    switch (format) {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
    NativeImageUtil::CopyNativeImage(&nativeImage, &mObjRenderImage);
}

void RealViewRender::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY,
                                        int degree, int mirror) {
    mTransformMatrix.degree = degree;
    mTransformMatrix.mirror = mirror;
}

void RealViewRender::UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix) {
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

//     float ratio = (float) mRenderImage.height / (float) mRenderImage.width;
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
//     glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
//    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 0.2), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(fFactorX * transformMatrix.scaleX,fFactorY * transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));

    mvpMatrix = Projection * View * Model;
}

void RealViewRender::UpdateObjMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix) {
//    float ratio = (float) mRenderImage.height / (float) mRenderImage.width;
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
//    glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
//    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 0.2), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(transformMatrix.scaleX, transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));

    mvpMatrix = Projection * View * Model;
}

RealViewRender *RealViewRender::GetInstance() {
    if (pInstance == nullptr) {
        pInstance = new RealViewRender();
    }
    return pInstance;
}

void RealViewRender::DestroyInstance() {
    if (pInstance) {
        delete pInstance;
        pInstance = nullptr;
    }
}
