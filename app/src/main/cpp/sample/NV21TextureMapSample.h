//
// Created by 徐极凯 on 2022/4/17.
//

#ifndef TESTOPENGL_NV21TEXTUREMAPSAMPLE_H
#define TESTOPENGL_NV21TEXTUREMAPSAMPLE_H

#include "GLSampleBase.h"

class NV21TextureMapSample: public GLSampleBase {
public:
    NV21TextureMapSample();

    ~NV21TextureMapSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void Destroy() override;

    void LoadImage(NativeImage *pImage) override;

private:
    GLuint m_yTextureId;
    GLuint m_uvTextureId;

    GLint m_ySamplerLoc;
    GLint m_uvSamplerLoc;

    NativeImage m_RenderImage;
};


#endif //TESTOPENGL_NV21TEXTUREMAPSAMPLE_H
