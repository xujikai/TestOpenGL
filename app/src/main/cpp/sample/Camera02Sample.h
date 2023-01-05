//
// Created by Administrator on 2023/1/4.
//

#ifndef TESTOPENGL_CAMERA02SAMPLE_H
#define TESTOPENGL_CAMERA02SAMPLE_H

#include "./GLCameraBase.h"
#include "../util/ByteFlowDef.h"
#include "../util/SyncLock.h"
#include <gtc/matrix_transform.hpp>

class Camera02Sample : public GLCameraBase {
public:
    ~Camera02Sample();

    int Init() override;

    int UnInit() override;

    void OnSurfaceCreated() override;

    void OnSurfaceChanged(int width, int height) override;

    void OnDrawFrame() override;

    void UpdateFrame(NativeImage *pImage) override;

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY,
                            int degree, int mirror) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix);

    void LoadShaderStr(int shaderIndex, char *pShaderStr, int strLen) override;

    void UpdateProgram();

private:
    GLint uMvpMatrixLoc;
    GLint uTextureYLoc;
    GLint uTextureULoc;
    GLint uTextureVLoc;

    GLuint mImageTexYId;
    GLuint mImageTexUId;
    GLuint mImageTexVId;
    NativeImage mRenderImage;

    glm::mat4 mMvpMatrix;
    TransformMatrix mTransformMatrix;

    std::string mFragShaderStr;
    volatile bool isShaderChanged;
    SyncLock mSyncLock;
};


#endif //TESTOPENGL_CAMERA02SAMPLE_H
