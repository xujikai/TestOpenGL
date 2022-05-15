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

    virtual void Destroy();

    virtual void Draw(int screenW, int screenH);

    void LoadImage(NativeImage *pImage);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc{};
    NativeImage m_RenderImage;
};

#endif //TESTOPENGL_TEXTUREMAPSAMPLE_H
