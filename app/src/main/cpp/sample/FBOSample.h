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

    void Destroy() override;

    void LoadImage(NativeImage *pImage) override;

    bool CreateFrameBufferObj();

private:
    GLuint m_FboProgramObj;
    GLuint m_FboVertexShader;
    GLuint m_FboFragmentShader;
    GLint m_FboSamplerLoc;
    GLuint m_FboTextureId;
    GLuint m_FboId;

    GLint m_SamplerLoc;
    GLuint m_TextureId;
    GLuint m_VaoIds[2];
    GLuint m_VboIds[4];
    NativeImage m_RenderImage;
};


#endif //TESTOPENGL_FBOSAMPLE_H
