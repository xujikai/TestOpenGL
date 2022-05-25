//
// Created by 徐极凯 on 2022/5/21.
//

#include "MediaCameraRender.h"
#include <gtc/matrix_transform.hpp>

static char vShaderStr[] =
        "#version 300 es\n"
        "layout(location = 0) in vec4 a_position;\n"
        "layout(location = 1) in vec2 a_texCoord;\n"
        "uniform mat4 u_MVPMatrix;\n"
        "out vec2 v_texCoord;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = u_MVPMatrix * a_position;\n"
        "    v_texCoord = a_texCoord;\n"
        "}";

static char fShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "in vec2 v_texCoord;\n"
        "layout(location = 0) out vec4 outColor;\n"
        "uniform sampler2D s_texture0;\n"
        "uniform sampler2D s_texture1;\n"
        "uniform sampler2D s_texture2;\n"
        "uniform int u_nImgType;// 1:RGBA, 2:NV21, 3:NV12, 4:I420\n"
        "\n"
        "void main()\n"
        "{\n"
        "\n"
        "    if(u_nImgType == 1) //RGBA\n"
        "    {\n"
        "        outColor = texture(s_texture0, v_texCoord);\n"
        "    }\n"
        "    else if(u_nImgType == 2) //NV21\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).a - 0.5;\n"
        "        yuv.z = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0, \t-0.344, \t1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "\n"
        "    }\n"
        "    else if(u_nImgType == 3) //NV12\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture1, v_texCoord).a - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "        0.0, \t-0.344, \t1.770,\n"
        "        1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else if(u_nImgType == 4) //I420\n"
        "    {\n"
        "        vec3 yuv;\n"
        "        yuv.x = texture(s_texture0, v_texCoord).r;\n"
        "        yuv.y = texture(s_texture1, v_texCoord).r - 0.5;\n"
        "        yuv.z = texture(s_texture2, v_texCoord).r - 0.5;\n"
        "        highp vec3 rgb = mat3(1.0,       1.0,     1.0,\n"
        "                              0.0, \t-0.344, \t1.770,\n"
        "                              1.403,  -0.714,     0.0) * yuv;\n"
        "        outColor = vec4(rgb, 1.0);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "        outColor = vec4(1.0);\n"
        "    }\n"
        "}";

static GLfloat verticesCoords[] = {
        -1.0f,  1.0f, 0.0f,  // Position 0
        -1.0f, -1.0f, 0.0f,  // Position 1
        1.0f,  -1.0f, 0.0f,  // Position 2
        1.0f,   1.0f, 0.0f,  // Position 3
};

static GLfloat textureCoords[] = {
        0.0f,  0.0f,        // TexCoord 0
        0.0f,  1.0f,        // TexCoord 1
        1.0f,  1.0f,        // TexCoord 2
        1.0f,  0.0f         // TexCoord 3
};

static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

MediaCameraRender *MediaCameraRender::m_pSample = nullptr;
std::mutex MediaCameraRender::m_Mutex;

MediaCameraRender::MediaCameraRender() {

}

MediaCameraRender::~MediaCameraRender() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void MediaCameraRender::Init(int videoWidth, int videoHeight, int *dstSize) {
    if(dstSize != nullptr) {
        dstSize[0] = videoWidth;
        dstSize[1] = videoHeight;
    }
    UpdateMVPMatrix(0, 0, 1.0f, 1.0f);
}

void MediaCameraRender::RenderVideoFrame(NativeImage *pImage) {
    LOGCATE("MediaCameraRender::RenderVideoFrame pImage=%p", pImage);
    if (pImage == nullptr || pImage->ppPlane[0] == nullptr) {
        return;
    }
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (pImage->width != m_RenderImage.width || pImage->height != m_RenderImage.height) {
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

void MediaCameraRender::UnInit() {

}

void MediaCameraRender::OnSurfaceCreated() {
    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);
    m_FboProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr);
    if (!m_ProgramObj || !m_FboProgramObj) {
        LOGCATE("MediaCameraRender::OnSurfaceCreated create program fail");
        return;
    }

    glGenTextures(TEXTURE_NUM, m_TextureIds);
    for (int i = 0; i < TEXTURE_NUM; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_TextureIds[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }

    glGenBuffers(3, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);

    glBindVertexArray(GL_NONE);
}

void MediaCameraRender::OnSurfaceChanged(int width, int height) {
    m_SurfaceWidth = width;
    m_SurfaceHeight = height;
    glViewport(0, 0, width, height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MediaCameraRender::OnDrawFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_ProgramObj == GL_NONE || m_RenderImage.ppPlane[0] == nullptr) return;
    if (m_SrcFboId == GL_NONE && CreateFrameBufferObj()) {
        LOGCATE("MediaCameraRender::OnDrawFrame CreateFrameBufferObj fail");
        return;
    }
    std::unique_lock<std::mutex> lock(m_Mutex);
    glBindFramebuffer(GL_FRAMEBUFFER, m_SrcFboId);
    if (m_TransformMatrix.degree == 0) {
        glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);
    } else { // 相机预览帧的宽和高反了，在进行离屏渲染时需要校正
        glViewport(0, 0, m_RenderImage.height, m_RenderImage.width);
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_FboProgramObj);

    if (m_TransformMatrix.degree == 0) {
        glBindTexture(GL_TEXTURE_2D, m_SrcFboTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, m_DstFboTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    } else {
        glBindTexture(GL_TEXTURE_2D, m_SrcFboTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.height, m_RenderImage.width, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindTexture(GL_TEXTURE_2D, m_DstFboTextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.height, m_RenderImage.width, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    switch (m_RenderImage.format) {
        case IMAGE_FORMAT_RGBA:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            break;
        case IMAGE_FORMAT_NV21:
        case IMAGE_FORMAT_NV12:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                         GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[1]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            break;
        case IMAGE_FORMAT_I420:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width, m_RenderImage.height, 0,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[1]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_TextureIds[2]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_RenderImage.width >> 1, m_RenderImage.height >> 1, 0,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[2]);
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            break;
    }

    glBindVertexArray(m_VaoId);
    UpdateMVPMatrix(&m_TransformMatrix);
    GLUtils::setMat4(m_FboProgramObj, "u_MVPMatrix", m_MVPMatrix);
    for (int i = 0; i < TEXTURE_NUM; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_TextureIds[i]);
        char samplerName[64] = {0};
        sprintf(samplerName, "s_texture%d", i);
        GLUtils::setInt(m_FboProgramObj, samplerName, i);
    }
//    GLUtils::setInt(m_FboProgramObj, "s_texture0", 0);
    GLUtils::setInt(m_FboProgramObj, "u_nImgType", m_RenderImage.format);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
//    LOGCATE("MediaCameraRender::OnDrawFrame %d %d %d", m_RenderImage.width, m_RenderImage.height, m_RenderImage.format);

    glBindFramebuffer(GL_FRAMEBUFFER, m_DstFboId);
    if (m_TransformMatrix.degree == 0) {
        glViewport(0, 0, m_RenderImage.width, m_RenderImage.height);
    } else { // 相机预览帧的宽和高反了，在进行离屏渲染时需要校正
        glViewport(0, 0, m_RenderImage.height, m_RenderImage.width);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoId);
    UpdateMVPMatrix(0, 0, 1.0, 1.0);
    GLUtils::setMat4(m_ProgramObj, "u_MVPMatrix", m_MVPMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_SrcFboTextureId);
    GLUtils::setInt(m_ProgramObj, "s_texture0", 0);
    GLUtils::setInt(m_ProgramObj, "u_nImgType", IMAGE_FORMAT_RGBA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    GetRenderFrameFromFBO();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    lock.unlock();

    glViewport(0, 0, m_SurfaceWidth, m_SurfaceHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    UpdateMVPMatrix(0, 0, 1.0, 1.0);
    GLUtils::setMat4(m_ProgramObj, "u_MVPMatrix", m_MVPMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_DstFboTextureId);
    GLUtils::setInt(m_ProgramObj, "s_texture0", 0);
    GLUtils::setInt(m_ProgramObj, "u_nImgType", IMAGE_FORMAT_RGBA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
//    LOGCATE("MediaCameraRender::OnDrawFrame %d %d", m_SurfaceWidth, m_SurfaceHeight);
}

void MediaCameraRender::UpdateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY) {
    angleX = angleX % 360;
    angleY = angleY % 360;

    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * angleY);

    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 4), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(scaleX, scaleY, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, 0.0f));

    m_MVPMatrix = Projection * View * Model;
}

void MediaCameraRender::UpdateMVPMatrix(TransformMatrix *pTransformMatrix) {
    m_TransformMatrix = *pTransformMatrix;

    //转化为弧度角
    float radiansX = static_cast<float>(MATH_PI / 180.0f * pTransformMatrix->angleX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * pTransformMatrix->angleY);

    float fFactorX = 1.0f;
    float fFactorY = 1.0f;

    if (pTransformMatrix->mirror == 1) {
        fFactorX = -1.0f;
    } else if (pTransformMatrix->mirror == 2) {
        fFactorY = -1.0f;
    }

    float fRotate = MATH_PI * pTransformMatrix->degree * 1.0f / 180;
    if (pTransformMatrix->mirror == 0) {
        if (pTransformMatrix->degree == 270) {
            fRotate = MATH_PI * 0.5;
        } else if (pTransformMatrix->degree == 180) {
            fRotate = MATH_PI;
        } else if (pTransformMatrix->degree == 90) {
            fRotate = MATH_PI * 1.5;
        }
    } else if (pTransformMatrix->mirror == 1) {
        if (pTransformMatrix->degree == 90) {
            fRotate = MATH_PI * 0.5;
        } else if (pTransformMatrix->degree == 180) {
            fRotate = MATH_PI;
        } else if (pTransformMatrix->degree == 270) {
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
    Model = glm::scale(Model, glm::vec3(fFactorX * pTransformMatrix->scaleX,
                                        fFactorY * pTransformMatrix->scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    Model = glm::translate(Model,glm::vec3(pTransformMatrix->translateX, pTransformMatrix->translateY, 0.0f));

    m_MVPMatrix = Projection * View * Model;
}

bool MediaCameraRender::CreateFrameBufferObj() {
    if (m_SrcFboTextureId == GL_NONE) {
        glGenTextures(1, &m_SrcFboTextureId);
        glBindTexture(GL_TEXTURE_2D, m_SrcFboTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    if (m_DstFboTextureId == GL_NONE) {
        glGenTextures(1, &m_DstFboTextureId);
        glBindTexture(GL_TEXTURE_2D, m_DstFboTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    if (m_SrcFboId == GL_NONE) {
        glGenFramebuffers(1, &m_SrcFboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_SrcFboId);
        glBindTexture(GL_TEXTURE_2D, m_SrcFboTextureId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SrcFboTextureId, 0);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.height, m_RenderImage.width, 0,
//                     GL_RGBA, GL_UNSIGNED_BYTE,nullptr);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,nullptr);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGCATE("MediaCameraRender::CreateFrameBufferObj srcFbo glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
            if (m_SrcFboTextureId != GL_NONE) {
                glDeleteTextures(1, &m_SrcFboTextureId);
                m_SrcFboTextureId = GL_NONE;
            }
            if (m_SrcFboId != GL_NONE) {
                glDeleteFramebuffers(1, &m_SrcFboId);
                m_SrcFboId = GL_NONE;
            }
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
            return false;
        }
    }
    if (m_DstFboId == GL_NONE) {
        glGenFramebuffers(1, &m_DstFboId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DstFboId);
        glBindTexture(GL_TEXTURE_2D, m_DstFboTextureId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_DstFboTextureId, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGCATE("MediaCameraRender::CreateFrameBufferObj dstFbo glCheckFramebufferStatus status != GL_FRAMEBUFFER_COMPLETE");
            if (m_DstFboTextureId != GL_NONE) {
                glDeleteTextures(1, &m_DstFboTextureId);
                m_DstFboTextureId = GL_NONE;
            }
            if (m_DstFboId != GL_NONE) {
                glDeleteFramebuffers(1, &m_DstFboId);
                m_DstFboId = GL_NONE;
            }
            glBindTexture(GL_TEXTURE_2D, GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
            return false;
        }
    }
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return true;
}

void MediaCameraRender::GetRenderFrameFromFBO() {
    if (m_RenderFrameCallback != nullptr) {
        uint8_t *pBuffer = new uint8_t[m_RenderImage.width * m_RenderImage.height * 4];
        NativeImage nativeImage = m_RenderImage;
        nativeImage.format = IMAGE_FORMAT_RGBA;
        if (m_TransformMatrix.degree == 0) {
            nativeImage.width = m_RenderImage.width;
            nativeImage.height = m_RenderImage.height;
        } else {
            nativeImage.width = m_RenderImage.height;
            nativeImage.height = m_RenderImage.width;
        }
        nativeImage.pLineSize[0] = nativeImage.width * 4;
        nativeImage.pLineSize[1] = 0;
        nativeImage.pLineSize[2] = 0;
        nativeImage.ppPlane[0] = pBuffer;
        nativeImage.ppPlane[1] = nullptr;
        nativeImage.ppPlane[2] = nullptr;
        glReadPixels(0, 0, nativeImage.width, nativeImage.height, GL_RGBA, GL_UNSIGNED_BYTE, pBuffer);
        LOGCATE("MediaCameraRender::GetRenderFrameFromFBO width=%d, height=%d, format=%d", nativeImage.width, nativeImage.height, nativeImage.format);
        LOGCATE("MediaCameraRender::GetRenderFrameFromFBO src[line0,line1,line2]=[%d, %d, %d]", nativeImage.pLineSize[0], nativeImage.pLineSize[1], nativeImage.pLineSize[2]);
        m_RenderFrameCallback(m_CallbackContext, &nativeImage);
        delete[] pBuffer;
    }
}

MediaCameraRender *MediaCameraRender::GetInstance() {
    if (m_pSample == nullptr) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_pSample == nullptr) {
            m_pSample = new MediaCameraRender();
        }
    }
    return m_pSample;
}

void MediaCameraRender::DestroyInstance() {
    if (m_pSample) {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_pSample) {
            delete m_pSample;
            m_pSample = nullptr;
        }
    }
}
