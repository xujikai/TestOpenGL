//
// Created by 徐极凯 on 2022/5/15.
//

#ifndef TESTOPENGL_MYCAMERARENDERCONTEXT_H
#define TESTOPENGL_MYCAMERARENDERCONTEXT_H

#include "../sample/GLCameraBase.h"

class MyCameraRenderContext {
    MyCameraRenderContext();

    ~MyCameraRenderContext();

public:
    void Init();

    void UnInit();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void UpdateFrame(int format, int width, int height, uint8_t *pData);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    static MyCameraRenderContext* GetInstance();

    static void DestroyInstance();

private:
    static MyCameraRenderContext *m_pContext;
    int m_ScreenW;
    int m_ScreenH;
    GLCameraBase *m_pSample;
};


#endif //TESTOPENGL_MYCAMERARENDERCONTEXT_H
