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
    GLSampleBase() {
        mProgramObj = GL_NONE;
        mSurfaceWidth = 0;
        mSurfaceHeight = 0;
    };
    virtual ~GLSampleBase() {};

    virtual void Init() = 0;

    virtual void Draw(int screenW, int screenH) = 0;

    virtual void UnInit() = 0;

    virtual void SetImageData(NativeImage *pImage) {};

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {};
protected:
    GLuint mProgramObj;
    int mSurfaceWidth;
    int mSurfaceHeight;
};

#endif //TESTOPENGL_GLSAMPLEBASE_H
