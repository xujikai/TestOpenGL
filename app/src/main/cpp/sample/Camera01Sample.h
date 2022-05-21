//
// Created by 徐极凯 on 2022/5/15.
//

#ifndef TESTOPENGL_CAMERA01SAMPLE_H
#define TESTOPENGL_CAMERA01SAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <GLES3/gl3.h>
#include "GLCameraBase.h"
#include "../util/GLUtils.h"
#include "../util/ByteFlowDef.h"

class Camera01Sample: public GLCameraBase {
public:
    Camera01Sample();

    ~Camera01Sample();

    int Init() override;

    int UnInit() override;

    void OnSurfaceCreated() override;

    void OnSurfaceChanged(int width, int height) override;

    void OnDrawFrame() override;

    void UpdateFrame(NativeImage *pImage) override;

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror) override;

    int CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);

    bool CreateTextures();

    bool UpdateTextures();

    GLuint UseProgram();

    void UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix);

private:
    GLuint m_Program;
    GLuint m_VertexShader;
    GLuint m_FragShader;

    GLuint m_YTextureId;
    GLuint m_UTextureId;
    GLuint m_VTextureId;

    GLuint m_VertexCoorHandle;
    GLuint m_TextureCoorHandle;
    GLint m_MVPHandle;
    GLint m_YTextureHandle;
    GLint m_UTextureHandle;
    GLint m_VTextureHandle;

    glm::mat4 m_MVPMatrix;
    TransformMatrix m_TransformMatrix;
};


#endif //TESTOPENGL_CAMERA01SAMPLE_H
