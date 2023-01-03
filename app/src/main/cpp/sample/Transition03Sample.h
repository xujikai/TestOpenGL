//
// Created by Administrator on 2023/1/3.
//

#ifndef TESTOPENGL_TRANSITION03SAMPLE_H
#define TESTOPENGL_TRANSITION03SAMPLE_H

#include "./GLSampleBase.h"
#include <gtc/matrix_transform.hpp>

#define RENDER_IMG_NUM 5
#define RENDER_LOOP_NUM 300

class Transition03Sample : public GLSampleBase {
public:
    ~Transition03Sample();

    virtual void Init() override;

    virtual void Draw(int screenW, int screenH) override;

    virtual void UnInit() override;

    void SetImageDataWithIndex(int index, NativeImage *pImage) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLint uMvpMatrixLoc;
    GLint uTexture0Loc;
    GLint uTexture1Loc;
    GLint uOffsetLoc;
    GLint uTexSizeLoc;

    GLuint mVaoId = GL_NONE;
    GLuint mVboIds[3] = {GL_NONE};

    GLuint mImageTexIds[RENDER_IMG_NUM] = {GL_NONE};
    NativeImage mRenderImages[RENDER_IMG_NUM];

    glm::mat4 mMvpMatrix;
    int mAngleX = 0;
    int mAngleY = 0;

    int mFrameIndex = 0;
    int mLoopNum = 0;
};


#endif //TESTOPENGL_TRANSITION03SAMPLE_H
