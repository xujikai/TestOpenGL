//
// Created by Administrator on 2022/8/5.
//

#ifndef TESTOPENGL_PBOSAMPLE_H
#define TESTOPENGL_PBOSAMPLE_H

#include <gtc/matrix_transform.hpp>
#include "GLSampleBase.h"

class PBOSample : public GLSampleBase{
public:
    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void UnInit();

    virtual void SetImageData(NativeImage *pImage);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    bool CreateFrameBufferObj();

    void UploadPixels();

    void DownloadPixels();

private:
    GLuint mProgram = GL_NONE;
    GLuint mFboProgram = GL_NONE;
    GLuint mFboId = GL_NONE;

    GLint uMvpMatrixLoc = GL_NONE;
    GLint uTextureLoc = GL_NONE;
    GLint uFboTextureLoc = GL_NONE;
    glm::mat4 mMvpMatrix = glm::mat4(1.0f);

    GLuint mVboIds[4] = {GL_NONE};
    GLuint mVaoIds[2] = {GL_NONE};

    GLuint mImageTexture = GL_NONE;
    GLuint mFboImageTexture = GL_NONE;
    NativeImage mRenderImage;

    GLuint mPboUploadIds[2] = {GL_NONE};
    GLuint mPboDownloadIds[2] = {GL_NONE};

    int mRotateX = 0;
    int mRotateY = 0;
    int mFrameIndex = 0;
};


#endif //TESTOPENGL_PBOSAMPLE_H
