//
// Created by 徐极凯 on 2022/5/15.
//

#ifndef TESTOPENGL_GLCAMERABASE_H
#define TESTOPENGL_GLCAMERABASE_H

#include "../util/ImageDef.h"
#include "../util/GLUtils.h"
#include <GLES3/gl3.h>

class GLCameraBase {
public:
    GLCameraBase() {};
    virtual ~GLCameraBase() {};

    virtual int Init() = 0;

    virtual int UnInit() = 0;

    virtual void OnSurfaceCreated() = 0;

    virtual void OnSurfaceChanged(int width, int height) = 0;

    virtual void OnDrawFrame() = 0;

    virtual void UpdateFrame(NativeImage *pImage) = 0;

    virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror) = 0;

    virtual void LoadShaderStr(int shaderIndex, char *pShaderStr, int strLen) {}

protected:
    volatile bool m_IsProgramChanged;
    GLuint mProgramObj = GL_NONE;
    int m_SurfaceWidth;
    int m_SurfaceHeight;
};

#endif //TESTOPENGL_GLCAMERABASE_H
