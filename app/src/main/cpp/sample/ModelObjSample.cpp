//
// Created by Administrator on 2022/8/4.
//

#include "ModelObjSample.h"

static char vShaderStr[] =
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

static char fShaderStr[] =
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
        "    outColor = vec4(finalColor, 1.0);\n"
        "}";

static char fNoTextureShaderStr[] =
        "#version 300 es\n"
        "precision highp float;\n"
        "out vec4 outColor;\n"
        "in vec3 ambient;\n"
        "in vec3 diffuse;\n"
        "in vec3 specular;\n"
        "void main()\n"
        "{    \n"
        "    vec4 objectColor = vec4(0.6, 0.6, 0.6, 1.0);\n"
        "    vec3 finalColor = (ambient + diffuse + specular) * vec3(objectColor);\n"
        "    outColor = vec4(finalColor, 1.0);\n"
        "}";

ModelObjSample::ModelObjSample() {
    mRotateX = 0;
    mRotateY = 0;
    mScaleX = 1.0f;
    mScaleY = 1.0f;
}

void ModelObjSample::Init() {
    std::string path = "/sdcard/Android/data/com.app.testopengl/files";
    pModel = new Model(path + "/model/poly/Apricot_02_hi_poly.obj");

    if (pModel->ContainsTextures()) {
        LOGCATE("new has texture shader");
        pShader = new Shader(vShaderStr, fShaderStr);
    } else {
        LOGCATE("new no texture shader");
        pShader = new Shader(vShaderStr, fNoTextureShaderStr);
    }
}

void ModelObjSample::Draw(int screenW, int screenH) {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mRotateY += 1;
    UpdateMVPMatrix(mMvpMatrix, mRotateX, mRotateY, (float) screenW / (float) screenH);

    pShader->Use();
    pShader->setMat4("u_MVPMatrix", mMvpMatrix);
    pShader->setMat4("u_ModelMatrix", mModelMatrix);
    pShader->setVec3("lightPos", glm::vec3(0, 0, pModel->GetMaxViewDistance()));
    pShader->setVec3("lightColor", glm::vec3(1.0, 1.0, 1.0));
    pShader->setVec3("viewPos", glm::vec3(0, 0, pModel->GetMaxViewDistance()));
    pModel->Draw(*pShader);
}

void ModelObjSample::UnInit() {
    if (pModel != nullptr) {
        pModel->Destroy();
        delete pModel;
        pModel = nullptr;
    }
    if (pShader != nullptr) {
        pShader->Destroy();
        delete pShader;
        pShader = nullptr;
    }
}

void ModelObjSample::SetImageData(NativeImage *pImage) {

}

void ModelObjSample::UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {
    mRotateX = static_cast<int>(rotateX);
    mRotateY = static_cast<int>(rotateY);
    mScaleX = scaleX;
    mScaleY = scaleY;
}

void ModelObjSample::UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio) {
    rotateX %= 360;
    rotateY %= 360;
    float radiansX = static_cast<float>(MATH_PI / 180.0f * rotateX);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * rotateY);

    glm::mat4 modelMatrix = glm::mat4(1.0);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(mScaleX, mScaleY, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, -pModel->GetAdjustModelPosVec());

    glm::mat4 viewMatrix = glm::lookAt(
            glm::vec3(0, 0, pModel->GetMaxViewDistance() * 1.8f),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0)
    );

    glm::mat4 projectionMatrix = glm::frustum(-ratio, ratio, -1.0f, 1.0f,
                                              1.0f,pModel->GetMaxViewDistance() * 4);

    mModelMatrix = modelMatrix;
    mMvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
}