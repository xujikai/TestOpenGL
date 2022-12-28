//
// Created by Administrator on 2022/12/28.
//

#ifndef TESTOPENGL_AVATARSAMPLE_H
#define TESTOPENGL_AVATARSAMPLE_H

#include "./GLSampleBase.h"
#include <gtc/matrix_transform.hpp>

static const int RENDER_IMG_NUM = 3;

class AvatarSample : public GLSampleBase {
public:
    ~AvatarSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageDataWithIndex(int index, NativeImage *pImage) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float transX, float transY, float ratio);

private:
    GLuint mBlurProgramObj;

    GLint uMvpMatrixLoc;
    GLint uTextureLoc;
    GLint uTexSizeLoc;
    GLint uNeedRotateLoc;
    GLint uRotateAngleLoc;

    GLint uBlurMvpMatrixLoc;
    GLint uBlurTextureLoc;

    GLuint mVaoId = GL_NONE;
    GLuint mVboIds[3] = {GL_NONE};

    GLuint mImageTexIds[RENDER_IMG_NUM] = {GL_NONE};
    NativeImage mRenderImages[RENDER_IMG_NUM];

    glm::mat4 mMvpMatrix;
    int mAngleX = 0;
    int mAngleY = 0;
    float mScaleX = 1.0f;
    float mScaleY = 1.0f;
    float mTransX = 0.0f;
    float mTransY = 0.0f;
    int mFrameIndex = 0;
};


#endif //TESTOPENGL_AVATARSAMPLE_H
