//
// Created by Administrator on 2022/12/29.
//

#ifndef TESTOPENGL_WATERRIPPLESAMPLE_H
#define TESTOPENGL_WATERRIPPLESAMPLE_H

#include "./GLSampleBase.h"
#include <gtc/matrix_transform.hpp>

class WaterRippleSample : public GLSampleBase {
public:
    ~WaterRippleSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageData(NativeImage *pImage) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLint uMvpMatrixLoc;
    GLint uTextureLoc;
    GLint uTouchXYLoc;
    GLint uTexSizeLoc;
    GLint uTimeLoc;
    GLint uBoundaryLoc;

    GLuint mVaoId = GL_NONE;
    GLuint mVboIds[3] = {GL_NONE};

    GLuint mImageTexId = GL_NONE;
    NativeImage mRenderImage;

    glm::mat4 mMvpMatrix;
    float mScaleX = 1.0f;
    float mScaleY = 1.0f;
    int mAngleX = 0;
    int mAngleY = 0;
    glm::vec2 mTouchXY = glm::vec2(0.5f, 0.5f);

    int mFrameIndex = 0;
};


#endif //TESTOPENGL_WATERRIPPLESAMPLE_H
