//
// Created by Administrator on 2023/1/6.
//

#ifndef TESTOPENGL_REALVIEWRENDER_H
#define TESTOPENGL_REALVIEWRENDER_H

#include <stdint.h>
#include <string>
#include <GLES3/gl3.h>
#include <gtc/matrix_transform.hpp>
#include "../util/ImageDef.h"
#include "../util/GLUtils.h"
#include "../util/ByteFlowDef.h"
#include "../model/Model.h"
#include "../model/Shader.h"

const static std::string vShaderStr = R"(
    #version 300 es
    layout(location = 0) in vec4 aPosition;
    layout(location = 1) in vec2 aTexCoord;
    uniform mat4 uMvpMatrix;
    out vec2 vTexCoord;

    void main() {
        gl_Position = uMvpMatrix * aPosition;
        vTexCoord = aTexCoord;
    }
)";

const static std::string fShaderStr = R"(
    #version 300 es
    precision mediump float;
    in vec2 vTexCoord;
    layout(location = 0) out vec4 outColor;
    uniform sampler2D uTextureY;
    uniform sampler2D uTextureU;
    uniform sampler2D uTextureV;

    void main() {
        float y, u, v, r, g, b;
        y = texture(uTextureY, vTexCoord).r;
        u = texture(uTextureU, vTexCoord).r;
        v = texture(uTextureV, vTexCoord).r;
        u = u - 0.5;
        v = v - 0.5;
        r = y + 1.403 * v;
        g = y - 0.344 * u - 0.714 * v;
        b = y + 1.770 * u;
        outColor = vec4(r, g, b, 1.0);
    }
)";

static char vObjShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 aPosition;   \n"
        "layout(location = 1) in vec2 aTexCoord;   \n"
        "uniform mat4 uMvpMatrix;                  \n"
        "out vec2 vTexCoord;                       \n"
        "void main()                                \n"
        "{                                          \n"
        "   gl_Position = uMvpMatrix * aPosition;   \n"
        "   vTexCoord = aTexCoord;                \n"
        "}                                          \n";
static char fObjShaderStr[] =
        "#version 300 es                                     \n"
        "precision mediump float;                            \n"
        "in vec2 vTexCoord;                                 \n"
        "layout(location = 0) out vec4 fragColor;             \n"
        "uniform sampler2D uTexture;                     \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  fragColor = texture(uTexture, vTexCoord);     \n"
        "  if (fragColor.a < 0.05) {\n"
        "     discard;\n"
        "  }\n"
        "}                                                   \n";

class RealViewRender {

    RealViewRender();

    ~RealViewRender();

public:
    void Init();

    void UnInit();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetObjImageData(int format, int width, int height, uint8_t *pData);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix);

    void UpdateObjMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix);

    static RealViewRender* GetInstance();

    static void DestroyInstance();

private:
    static RealViewRender *pInstance;
    GLuint mProgramObj = GL_NONE;
    int mSurfaceWidth;
    int mSurfaceHeight;

    GLint uMvpMatrixLoc;
    GLint uTextureYLoc;
    GLint uTextureULoc;
    GLint uTextureVLoc;

    GLuint mImageTexYId = GL_NONE;
    GLuint mImageTexUId = GL_NONE;
    GLuint mImageTexVId = GL_NONE;
    NativeImage mRenderImage;

    glm::mat4 mMvpMatrix;
    TransformMatrix mTransformMatrix;

    GLuint mObjProgramObj = GL_NONE;
    GLint uObjTextureLoc = GL_NONE;
    GLint uObjMvpMatrixLoc = GL_NONE;
    GLuint mObjImageTexId = GL_NONE;
    NativeImage mObjRenderImage;
    glm::mat4 mObjMvpMatrix;
};

#endif //TESTOPENGL_REALVIEWRENDER_H
