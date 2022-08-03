//
// Created by 徐极凯 on 2022/4/17.
//

#ifndef TESTOPENGL_TEXTUREMAPSAMPLE_H
#define TESTOPENGL_TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"

class TextureMapSample: public GLSampleBase {
public:
    TextureMapSample();

    ~TextureMapSample();

    virtual void Init();

    virtual void UnInit();

    virtual void Draw(int screenW, int screenH);

    void SetImageData(NativeImage *pImage);

private:
    GLint aPositionLoc;
    GLint aTexCoordLoc;
    GLint uTextureLoc;
    GLuint mImageTexture;
    NativeImage mRenderImage;
};

#endif //TESTOPENGL_TEXTUREMAPSAMPLE_H
