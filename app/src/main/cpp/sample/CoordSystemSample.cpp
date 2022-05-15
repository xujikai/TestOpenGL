//
// Created by 徐极凯 on 2022/5/3.
//

#include <gtc/matrix_transform.hpp>
#include "CoordSystemSample.h"

CoordSystemSample::CoordSystemSample() {
    m_TextureId = GL_NONE;
    m_VaoId = GL_NONE;
    m_SamplerLoc = GL_NONE;
    m_MVPMatrixLoc = GL_NONE;

    m_RotateX = 0;
    m_RotateY = 80;
    m_ScaleX = 1.0f;
    m_ScaleY = 1.0f;
}

CoordSystemSample::~CoordSystemSample() {
    NativeImageUtil::FreeNativeImage(&m_RenderImage);
}

void CoordSystemSample::Init() {
    if (m_ProgramObj != GL_NONE) return;

    char vShaderStr[] =
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

    char fShaderStr[] =
            "#version 300 es                                     \n"
            "precision mediump float;                            \n"
            "in vec2 v_texCoord;                                 \n"
            "layout(location = 0) out vec4 outColor;             \n"
            "uniform sampler2D s_TextureMap;                     \n"
            "void main()                                         \n"
            "{                                                   \n"
            "  outColor = texture(s_TextureMap, v_texCoord);     \n"
            "}                                                   \n";

    m_ProgramObj = GLUtils::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
        m_MVPMatrixLoc = glGetUniformLocation(m_ProgramObj, "u_MVPMatrix");
    } else {
        LOGCATE("CoordSystemSample::Init create program fail");
    }

    GLfloat verticesCoords[] = {
            -1.0f,  1.0f, 0.0f,  // Position 0
            -1.0f, -1.0f, 0.0f,  // Position 1
            1.0f,  -1.0f, 0.0f,  // Position 2
            1.0f,   1.0f, 0.0f,  // Position 3
    };

    GLfloat textureCoords[] = {
            0.0f,  0.0f,        // TexCoord 0
            0.0f,  1.0f,        // TexCoord 1
            1.0f,  1.0f,        // TexCoord 2
            1.0f,  0.0f         // TexCoord 3
    };

    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // 生成 VBO 并绑定数据
    glGenBuffers(3, m_VboIds);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 生成 VAO 并绑定数据
    glGenVertexArrays(1, &m_VaoId);
    glBindVertexArray(m_VaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (const void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ARRAY_BUFFER, m_VboIds[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const void *)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VboIds[2]);
    glBindVertexArray(GL_NONE);

    // 生成纹理
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 给纹理赋值图像数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_RenderImage.width, m_RenderImage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_RenderImage.ppPlane[0]);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void CoordSystemSample::Draw(int screenW, int screenH) {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) return;

    UpdateMVPMatrix(m_MVPMatrix, m_RotateX, m_RotateY, (float)screenW / (float)screenH);

    glUseProgram(m_ProgramObj);
    glBindVertexArray(m_VaoId);

    glUniformMatrix4fv(m_MVPMatrixLoc, 1, GL_FALSE, &m_MVPMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glUniform1i(m_SamplerLoc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (const void *)0);

    glBindVertexArray(GL_NONE);
}

void CoordSystemSample::Destroy() {
    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        glDeleteBuffers(3, m_VboIds);
        glDeleteVertexArrays(1, &m_VaoId);
        glDeleteTextures(1, &m_TextureId);
    }
}

void CoordSystemSample::LoadImage(NativeImage *pImage) {
    if (pImage) {
        m_RenderImage.width = pImage->width;
        m_RenderImage.height = pImage->height;
        m_RenderImage.format = pImage->format;
        NativeImageUtil::CopyNativeImage(pImage, &m_RenderImage);
    }
}

void CoordSystemSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    m_RotateX = static_cast<int>(rotateX);
    m_RotateY = static_cast<int>(rotateY);

    m_ScaleX = scaleX;
    m_ScaleY = scaleY;
}

void CoordSystemSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio) {
    rotateX = rotateX % 360;
    rotateY = rotateY % 360;

    float radiansX = static_cast<float>(MATH_PI / 180 * rotateX);
    float radiansY = static_cast<float>(MATH_PI / 180 * rotateY);

    // 投影矩阵
    glm::mat4 ProjectionMat = glm::perspective(45.0f, ratio, 0.1f, 100.0f);

    // 视图矩阵
    glm::mat4 ViewMat = glm::lookAt(
            glm::vec3(0, 0, 4),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
            );

    // 模型矩阵
    glm::mat4 ModelMat = glm::mat4(1.0f);
    ModelMat = glm::scale(ModelMat, glm::vec3(m_ScaleX, m_ScaleY, 1.0f));
    ModelMat = glm::rotate(ModelMat, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    ModelMat = glm::rotate(ModelMat, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));

    mvpMatrix = ProjectionMat * ViewMat * ModelMat;
}
