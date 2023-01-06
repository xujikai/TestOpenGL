//
// Created by Administrator on 2023/1/6.
//

#include "RealViewRenderBak.h"

RealViewRenderBak *RealViewRenderBak::pInstance = nullptr;

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

RealViewRenderBak::RealViewRenderBak() {

}

RealViewRenderBak::~RealViewRenderBak() {

}

void RealViewRenderBak::Init() {

}

void RealViewRenderBak::UnInit() {
    if (mProgramObj) {
        glDeleteProgram(mProgramObj);
        glDeleteTextures(1, &mImageTexYId);
        glDeleteTextures(1, &mImageTexUId);
        glDeleteTextures(1, &mImageTexVId);
        mProgramObj = GL_NONE;
    }
    if (pModel != nullptr) {
        pModel->Destroy();
        delete pModel;
        pModel = nullptr;
    }
    if (pShader != nullptr) {
        pShader->Destroy();
        delete pShader;
        pShader = nullptr;
    }
    DestroyInstance();
}

void RealViewRenderBak::OnSurfaceCreated() {
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

    std::string path = "/sdcard/Android/data/com.app.testopengl/files";
    pModel = new Model(path + "/model/qianyu/qianyu.gltf");
    pShader = new Shader(vObjShaderStr, fObjShaderStr);
}

void RealViewRenderBak::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    mSurfaceWidth = width;
    mSurfaceHeight = height;
}

void RealViewRenderBak::OnDrawFrame() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

//    if (pShader == nullptr || pModel == nullptr) return;
//    UpdateObjMVPMatrix(mObjMvpMatrix, 0, 0, (float) mSurfaceWidth / (float) mSurfaceHeight);
//    pShader->Use();
//    pShader->setMat4("u_MVPMatrix", mObjMvpMatrix);
//    pShader->setMat4("u_ModelMatrix", mObjModelMatrix);
//    pShader->setVec3("lightPos", glm::vec3(0, 0, pModel->GetMaxViewDistance()));
//    pShader->setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
//    pShader->setVec3("viewPos", glm::vec3(0, 0, pModel->GetMaxViewDistance()));
//    pModel->Draw(*pShader);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void RealViewRenderBak::SetImageData(int format, int width, int height, uint8_t *pData) {
//    NativeImageUtil::FreeNativeImage(&mRenderImage);
    mRenderImage.format = format;
    mRenderImage.width = width;
    mRenderImage.height = height;
    mRenderImage.ppPlane[0] = pData;

    switch (format) {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            mRenderImage.ppPlane[1] = mRenderImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            mRenderImage.ppPlane[1] = mRenderImage.ppPlane[0] + width * height;
            mRenderImage.ppPlane[2] = mRenderImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }
}

void RealViewRenderBak::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY,
                                        int degree, int mirror) {
    mTransformMatrix.degree = degree;
    mTransformMatrix.mirror = mirror;
}

void RealViewRenderBak::UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix) {
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

    float ratio = (float) mRenderImage.height / (float) mRenderImage.width;
//    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 0.1f, 100.0f);
//    glm::mat4 Projection = glm::perspective(45.0f, ratio, 0.1f, 100.f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 0.2), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(fFactorX * transformMatrix.scaleX,
                                        fFactorY * transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));
    LOGCATE("camera matrix %f, %f, %f", transformMatrix.translateX, transformMatrix.translateY, 0.0f);

    mvpMatrix = Projection * View * Model;
}

void RealViewRenderBak::UpdateObjMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio) {
    rotateX %= 360;
    rotateY %= 360;
    float radiansX = static_cast<float>(MATH_PI / 180.0f * rotateX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * rotateY);

    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, -pModel->GetAdjustModelPosVec());
    LOGCATE("obj matrix %f, %f, %f", -pModel->GetAdjustModelPosVec()[0], -pModel->GetAdjustModelPosVec()[1],
            -pModel->GetAdjustModelPosVec()[2]);

    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 0, pModel->GetMaxViewDistance() * 1.8f),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    glm::mat4 projectionMatrix = glm::frustum(-ratio, ratio, -1.0f, 1.0f,
                                              1.0f,pModel->GetMaxViewDistance() * 4);

    mObjModelMatrix = modelMatrix;
    mObjMvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}

RealViewRenderBak *RealViewRenderBak::GetInstance() {
    if (pInstance == nullptr) {
        pInstance = new RealViewRenderBak();
    }
    return pInstance;
}

void RealViewRenderBak::DestroyInstance() {
    if (pInstance) {
        delete pInstance;
        pInstance = nullptr;
    }
}