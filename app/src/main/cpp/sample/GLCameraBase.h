//
// Created by 徐极凯 on 2022/5/15.
//

#ifndef TESTOPENGL_GLCAMERABASE_H
#define TESTOPENGL_GLCAMERABASE_H

#include "../util/ImageDef.h"
#include "../util/LogUtil.h"

class GLCameraBase {
public:
    GLCameraBase() {}
    ~GLCameraBase() {}

    virtual int Init() = 0;

    virtual int UnInit() = 0;

    virtual void OnSurfaceCreated() = 0;

    virtual void OnSurfaceChanged(int width, int height) = 0;

    virtual void OnDrawFrame() = 0;

    virtual void UpdateFrame(NativeImage *pImage) = 0;

    virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror) = 0;

protected:
    volatile bool m_IsProgramChanged;
    int m_SurfaceWidth;
    int m_SurfaceHeight;
    NativeImage m_RenderImage;
};

#endif //TESTOPENGL_GLCAMERABASE_H
