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
        m_ProgramObj = GL_NONE;
        m_VertexShader = GL_NONE;
        m_FragmentShader = GL_NONE;

        m_SurfaceWidth = 0;
        m_SurfaceHeight = 0;
    };
    virtual ~GLSampleBase() {};

    virtual void Init() = 0;

    virtual void Draw(int screenW, int screenH) = 0;

    virtual void Destroy() = 0;

    virtual void LoadImage(NativeImage *pImage) {};

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {};
protected:
    GLuint m_ProgramObj;
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    int m_SurfaceWidth;
    int m_SurfaceHeight;
};

#endif //TESTOPENGL_GLSAMPLEBASE_H
