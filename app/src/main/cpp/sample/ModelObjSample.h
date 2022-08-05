//
// Created by Administrator on 2022/8/4.
//

#ifndef TESTOPENGL_MODELOBJSAMPLE_H
#define TESTOPENGL_MODELOBJSAMPLE_H

#include <gtc/matrix_transform.hpp>
#include "GLSampleBase.h"
#include "../model/Model.h"
#include "../model/Shader.h"

class ModelObjSample : public GLSampleBase {

public:
    ModelObjSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageData(NativeImage *pImage) override;

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio);

private:
    Model *pModel;
    Shader *pShader;

    glm::mat4 mMvpMatrix;
    glm::mat4 mModelMatrix;

    int mRotateX;
    int mRotateY;
    float mScaleX;
    float mScaleY;
};


#endif //TESTOPENGL_MODELOBJSAMPLE_H
