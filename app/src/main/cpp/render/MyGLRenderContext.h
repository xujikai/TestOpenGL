//
// Created by 徐极凯 on 2022/4/16.
//

#ifndef TESTOPENGL_MYGLRENDERCONTEXT_H
#define TESTOPENGL_MYGLRENDERCONTEXT_H

#include "../sample/GLSampleBase.h"

class MyGLRenderContext {
    MyGLRenderContext();

    ~MyGLRenderContext();

public:
    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetImageDataWithIndex(int index, int format, int width, int height, uint8_t *pData);

    static MyGLRenderContext* GetInstance();

    static void DestroyInstance();

private:
    static MyGLRenderContext *m_pContext;
    int m_ScreenW;
    int m_ScreenH;
    GLSampleBase *m_pSample;
};


#endif //TESTOPENGL_MYGLRENDERCONTEXT_H
