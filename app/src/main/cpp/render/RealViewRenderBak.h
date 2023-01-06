//
// Created by Administrator on 2023/1/6.
//

#ifndef TESTOPENGL_REALVIEWRENDERBAK_H
#define TESTOPENGL_REALVIEWRENDERBAK_H

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
        "#version 300 es\n"
        "precision mediump float;\n"
        "layout (location = 0) in vec3 a_position;\n"
        "layout (location = 1) in vec3 a_normal;\n"
        "layout (location = 2) in vec2 a_texCoord;\n"
        "out vec2 v_texCoord;\n"
        "uniform mat4 u_MVPMatrix;\n"
        "uniform mat4 u_ModelMatrix;\n"
        "uniform vec3 lightPos;\n"
        "uniform vec3 lightColor;\n"
        "uniform vec3 viewPos;\n"
        "out vec3 ambient;\n"
        "out vec3 diffuse;\n"
        "out vec3 specular;\n"
        "void main()\n"
        "{\n"
        "    v_texCoord = a_texCoord;    \n"
        "    vec4 position = vec4(a_position, 1.0);\n"
        "    gl_Position = u_MVPMatrix * position;\n"
        "    vec3 fragPos = vec3(u_ModelMatrix * position);\n"
        "\n"
        "    // Ambient\n"
        "    float ambientStrength = 0.25;\n"
        "    ambient = ambientStrength * lightColor;\n"
        "\n"
        "    // Diffuse\n"
        "    float diffuseStrength = 0.5;\n"
        "    vec3 unitNormal = normalize(vec3(u_ModelMatrix * vec4(a_normal, 1.0)));\n"
        "    vec3 lightDir = normalize(lightPos - fragPos);\n"
        "    float diff = max(dot(unitNormal, lightDir), 0.0);\n"
        "    diffuse = diffuseStrength * diff * lightColor;\n"
        "\n"
        "    // Specular\n"
        "    float specularStrength = 0.3;\n"
        "    vec3 viewDir = normalize(viewPos - fragPos);\n"
        "    vec3 reflectDir = reflect(-lightDir, unitNormal);\n"
        "    float spec = pow(max(dot(unitNormal, reflectDir), 0.0), 16.0);\n"
        "    specular = specularStrength * spec * lightColor;\n"
        "}";

static char fObjShaderStr[] =
        "#version 300 es\n"
        "precision mediump float;"
        "out vec4 outColor;\n"
        "in vec2 v_texCoord;\n"
        "in vec3 ambient;\n"
        "in vec3 diffuse;\n"
        "in vec3 specular;\n"
        "uniform sampler2D texture_diffuse1;\n"
        "void main()\n"
        "{    \n"
        "    vec4 objectColor = texture(texture_diffuse1, v_texCoord);\n"
        "    vec3 finalColor = (ambient + diffuse + specular) * vec3(objectColor);\n"
        "    outColor = vec4(finalColor, 0.5);\n"
        "}";

//"    if (objectColor.a > 0.5) {\n"
//"       outColor = vec4(finalColor, 1.0);\n"
//"    } else {\n"
//"       discard;\n"
//"    }\n"
// "       outColor = vec4(finalColor, 1.0);\n"

class RealViewRenderBak {
    RealViewRenderBak();

    ~RealViewRenderBak();

public:
    void Init();

    void UnInit();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void SetImageData(int format, int width, int height, uint8_t *pData);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, TransformMatrix &transformMatrix);

    void UpdateObjMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio);

    static RealViewRenderBak* GetInstance();

    static void DestroyInstance();

private:
    static RealViewRenderBak *pInstance;
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

    Model *pModel = nullptr;
    Shader *pShader = nullptr;
    glm::mat4 mObjMvpMatrix;
    glm::mat4 mObjModelMatrix;
};


#endif //TESTOPENGL_REALVIEWRENDERBAK_H
