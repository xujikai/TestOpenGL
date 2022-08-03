//
// Created by 徐极凯 on 2022/4/17.
//

#ifndef TESTOPENGL_FBOSAMPLE_H
#define TESTOPENGL_FBOSAMPLE_H

#include "GLSampleBase.h"

class FBOSample: public GLSampleBase {

public:
    FBOSample();

    ~FBOSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageData(NativeImage *pImage) override;

    bool CreateFrameBufferObj();

private:
    GLuint mFboProgramObj;
    GLuint mFboId;

    GLuint aPositionLoc;
    GLuint aFboPositionLoc;
    GLuint aTexCoordLoc;
    GLuint aFboTexCoordLoc;
    GLint uTextureLoc;
    GLint uFboTextureLoc;

    GLuint mVaoIds[2];
    GLuint mVboIds[4];

    GLuint mImageTexture;
    GLuint mFboTexture;

    NativeImage mRenderImage;
};


#endif //TESTOPENGL_FBOSAMPLE_H
