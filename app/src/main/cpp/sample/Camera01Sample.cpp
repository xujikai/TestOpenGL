//
// Created by 徐极凯 on 2022/5/15.
//

#include "Camera01Sample.h"
#include <gtc/matrix_transform.hpp>

static const char kVertexShader[] =
        "#version 100\n"
        "varying vec2 v_texcoord;\n"
        "attribute vec4 position;\n"
        "attribute vec2 texcoord;\n"
        "uniform mat4 MVP;\n"
        "void main() {\n"
        "    v_texcoord = texcoord;\n"
        "    gl_Position = MVP*position;\n"
        "}";

// Pixel shader, YUV420 to RGB conversion.
static const char kFragmentShader0[] =
        "#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    void main() {\
        float y, u, v, r, g, b;\
        y = texture2D(s_textureY, v_texcoord).r;\
        u = texture2D(s_textureU, v_texcoord).r;\
        v = texture2D(s_textureV, v_texcoord).r;\
        u = u - 0.5;\
        v = v - 0.5;\
        r = y + 1.403 * v;\
        g = y - 0.344 * u - 0.714 * v;\
        b = y + 1.770 * u;\
        gl_FragColor = vec4(r, g, b, 1.0);\
    }";

static const float VERTICES_COORS[] =
        {
                -1.f, 1.f,
                -1.f, -1.f,
                1.f, 1.f,
                1.f, -1.f
        };

static const float TEXTURE_COORS[] =
        {
                0, 0,
                0, 1,
                1, 0,
                1, 1
        };

Camera01Sample::Camera01Sample() {
    LOGCATE("Camera01Sample::Camera01Sample");
    m_IsProgramChanged = false;

    m_Program = 0;
    m_VertexShader = 0;
    m_FragShader = 0;

    m_YTextureId = 0;
    m_UTextureId = 0;
    m_VTextureId = 0;

    m_VertexCoorHandle = 0;
    m_TextureCoorHandle = 0;
    m_YTextureHandle = 0;
    m_UTextureHandle = 0;
    m_VTextureHandle = 0;
}

Camera01Sample::~Camera01Sample() {
    LOGCATE("Camera01Sample::~Camera01Sample");
}

int Camera01Sample::Init() {
    LOGCATE("Camera01Sample::Init");
    return 0;
}

int Camera01Sample::UnInit() {
    LOGCATE("Camera01Sample::UnInit");
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
    return 0;
}

void Camera01Sample::OnSurfaceCreated() {
    CreateProgram(kVertexShader, kFragmentShader0);
}

void Camera01Sample::OnSurfaceChanged(int width, int height) {
    m_SurfaceWidth = width;
    m_SurfaceHeight = height;
    m_IsProgramChanged = true;
}

void Camera01Sample::OnDrawFrame() {
    glViewport(0, 0, m_SurfaceWidth, m_SurfaceHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    if (!UpdateTextures() || !UseProgram()) {
        LOGCATE("GLByteFlowRender::OnDrawFrame skip frame");
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Camera01Sample::UpdateFrame(NativeImage *pImage) {
    LOGCATE("Camera01Sample::UpdateFrame");
    if (pImage == nullptr) return;
    if (pImage->width != m_RenderImage.width || pImage->height != m_RenderImage.height) {
        LOGCATE("Camera01Sample::UpdateFrame::SetValue");
        if (m_RenderImage.ppPlane[0] != nullptr) {
            NativeImageUtil::FreeNativeImage(&m_RenderImage);
        }
        memset(&m_RenderImage, 0, sizeof(NativeImage));
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::AllocNativeImage(&m_RenderImage);
    }
    NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
}

void Camera01Sample::SetTransformMatrix(float translateX, float translateY, float scaleX,
                                        float scaleY, int degree, int mirror) {
    LOGCATE("Camera01Sample::SetTransformMatrix translateX = %f, translateY = %f, scaleX = %f, scaleY = %f, degree = %d, mirror = %d",
            translateX, translateY, scaleX, scaleY, degree, mirror);
    m_TransformMatrix.degree = degree;
    m_TransformMatrix.mirror = mirror;
    m_IsProgramChanged = true;
}

int Camera01Sample::CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource) {
    LOGCATE("Camera01Sample::CreateProgram");
    m_Program = GLUtils::CreateProgram(pVertexShaderSource, pFragShaderSource, m_VertexShader, m_FragShader);

    m_YTextureHandle = glGetUniformLocation(m_Program, "s_textureY");
    m_UTextureHandle = glGetUniformLocation(m_Program, "s_textureU");
    m_VTextureHandle = glGetUniformLocation(m_Program, "s_textureV");

    m_VertexCoorHandle = glGetAttribLocation(m_Program, "position");
    m_TextureCoorHandle = glGetAttribLocation(m_Program, "texcoord");
    m_MVPHandle = glGetUniformLocation(m_Program, "MVP");

    return m_Program;
}

bool Camera01Sample::CreateTextures() {
    LOGCATE("Camera01Sample::CreateTextures");
    GLsizei yWidth = m_RenderImage.width;
    GLsizei yHeight = m_RenderImage.height;

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_YTextureId);
    glBindTexture(GL_TEXTURE_2D, m_YTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yWidth, yHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    if (!m_YTextureId) {
        GLUtils::CheckGLError("GLByteFlowRender::CreateTextures Create Y texture");
        return false;
    }

    GLsizei uWidth = m_RenderImage.width / 2;
    GLsizei uHeight = yHeight / 2;

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &m_UTextureId);
    glBindTexture(GL_TEXTURE_2D, m_UTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, uWidth, uHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    if (!m_UTextureId) {
        GLUtils::CheckGLError("GLByteFlowRender::CreateTextures Create U texture");
        return false;
    }

    GLsizei vWidth = m_RenderImage.width / 2;
    GLsizei vHeight = yHeight / 2;

    glGenTextures(1, &m_VTextureId);
    glBindTexture(GL_TEXTURE_2D, m_VTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, vWidth, vHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    if (!m_VTextureId) {
        GLUtils::CheckGLError("GLByteFlowRender::CreateTextures Create V texture");
        return false;
    }
    return true;
}

bool Camera01Sample::UpdateTextures() {
    LOGCATE("Camera01Sample::UpdateTextures");
    if (m_RenderImage.ppPlane[0] == nullptr) {
        return false;
    }
    if (!m_YTextureId && !m_UTextureId && !m_VTextureId && !CreateTextures()) {
        return false;
    }

    LOGCATE("Camera01Sample::UpdateTextures::GenImage");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_YTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_UTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_VTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[2]);

    return true;
}

GLuint Camera01Sample::UseProgram() {
    LOGCATE("Camera01Sample::UseProgram");
    if (!m_Program) {
        LOGCATE("GLByteFlowRender::UseProgram Could not use program.");
        return 0;
    }
    if (m_IsProgramChanged) {
        LOGCATE("Camera01Sample::UseProgram::SetValue");
        glUseProgram(m_Program);

        glVertexAttribPointer(m_VertexCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), VERTICES_COORS);
        glEnableVertexAttribArray(m_VertexCoorHandle);

        glUniform1i(m_YTextureHandle, 0);
        glUniform1i(m_UTextureHandle, 1);
        glUniform1i(m_VTextureHandle, 2);

        glVertexAttribPointer(m_TextureCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), TEXTURE_COORS);
        glEnableVertexAttribArray(m_TextureCoorHandle);

        m_IsProgramChanged = false;
    }
    UpdateMVPMatrix(m_MVPMatrix, m_TransformMatrix);
    glUniformMatrix4fv(m_MVPHandle, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    return m_Program;
}

void Camera01Sample::UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix) {
    float fFactorX = 1.0f;
    float fFactorY = 1.0f;

    if (transformMatrix.mirror == 1) {
        fFactorX = -1.0f;
    } else if (transformMatrix.mirror == 2) {
        fFactorY = -1.0f;
    }

    float fRotate = MATH_PI * transformMatrix.degree * 1.0f / 180;
    if (transformMatrix.mirror == 0) {
        if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 1.5;
        }
    } else if (transformMatrix.mirror == 1) {
        if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 1.5;
        }
    }

    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(fFactorX * transformMatrix.scaleX,
                                        fFactorY * transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));

    mat4Matrix = Projection * View * Model;
}
