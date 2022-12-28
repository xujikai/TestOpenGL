//
// Created by 徐极凯 on 2022/4/16.
//

#ifndef TESTOPENGL_GLSAMPLEBASE_H
#define TESTOPENGL_GLSAMPLEBASE_H

#include "../util/ImageDef.h"
#include "../util/GLUtils.h"
#include <GLES3/gl3.h>

class GLSampleBase {
public:
    GLSampleBase() {};
    virtual ~GLSampleBase() {};

    virtual void Init() = 0;

    virtual void Draw(int screenW, int screenH) = 0;

    virtual void UnInit() = 0;

    virtual void SetImageData(NativeImage *pImage) {};

    virtual void SetImageDataWithIndex(int index, NativeImage *pImage) {};

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {};
protected:
    GLuint mProgramObj = GL_NONE;
    int mSurfaceWidth = 0;
    int mSurfaceHeight = 0;
};

#endif //TESTOPENGL_GLSAMPLEBASE_H
