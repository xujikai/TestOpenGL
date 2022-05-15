//
// Created by 徐极凯 on 2022/4/21.
//

#ifndef TESTOPENGL_MYEGLRENDERCONTEXT_H
#define TESTOPENGL_MYEGLRENDERCONTEXT_H

#include "../util/ImageDef.h"
#include "../util/GLUtils.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#define EGL_FEATURE_NUM 8

class MyEGLRenderContext {

    MyEGLRenderContext();

    ~MyEGLRenderContext();

public:
    void Init();

    void Draw();

    void UnInit();

    int CreateGLESEnv();

    void DestroyGLESEnv();

    void SetImageData(int width, int height, uint8_t *pData);

    static MyEGLRenderContext *GetInstance();

    static void DestroyInstance();

private:
    static MyEGLRenderContext *m_Instance;
    GLuint m_ProgramObj;
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    GLuint m_TextureId;
    GLint m_SamplerLoc;
    GLint m_TexSizeLoc;
    NativeImage m_RenderImage;

    GLuint m_FboTextureId;
    GLuint m_FboId;
    GLuint m_VaoIds[1] = { GL_NONE };
    GLuint m_VboIds[3] = { GL_NONE };

    EGLConfig m_EglConfig;
    EGLSurface m_EglSurface;
    EGLContext m_EglContext;
    EGLDisplay m_EglDisplay;
    bool m_IsGlContextReady;
    int m_ShaderIndex;
    const char *m_ShaderStrArr[EGL_FEATURE_NUM];
};


#endif //TESTOPENGL_MYEGLRENDERCONTEXT_H
