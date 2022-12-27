//
// Created by 徐极凯 on 2022/8/13.
//

#ifndef TESTOPENGL_BLITSAMPLE_H
#define TESTOPENGL_BLITSAMPLE_H

#include <gtc/matrix_transform.hpp>
#include "./GLSampleBase.h"

class BlitSample : public GLSampleBase {

    ~BlitSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageData(NativeImage *pImage) override;

    void CreateFrameBufferObj();

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint mFboId = GL_NONE;

    GLint uMvpMatrixLoc = GL_NONE;
    GLint uTextureLoc = GL_NONE;

    GLuint mVaoId = GL_NONE;
    GLuint mVboIds[3] = {GL_NONE};

    GLuint mImageTexId = GL_NONE;
    GLuint mFboImageTexIds[4] = {GL_NONE};

    NativeImage mRenderImage;

    glm::mat4 mMvpMatrix = glm::mat4(1.0f);
    int mRotateX = 0;
    int mRotateY = 0;
};


#endif //TESTOPENGL_BLITSAMPLE_H
