//
// Created by 徐极凯 on 2022/5/3.
//

#ifndef TESTOPENGL_COORDSYSTEMSAMPLE_H
#define TESTOPENGL_COORDSYSTEMSAMPLE_H

#include "GLSampleBase.h"
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>

class CoordSystemSample: public GLSampleBase {
public:
    CoordSystemSample();

    ~CoordSystemSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void SetImageData(NativeImage *pImage) override;

    void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio);

private:
    GLint aPositionLoc;
    GLint aTexCoordLoc;
    GLint uTextureLoc;
    GLint uMVPMatrixLoc;

    GLuint mVaoId;
    GLuint mVboIds[3];

    GLuint mImageTexture;
    NativeImage mRenderImage;
    glm::mat4 mMVPMatrix;

    int mRotateX;
    int mRotateY;
    float mScaleX;
    float mScaleY;
};


#endif //TESTOPENGL_COORDSYSTEMSAMPLE_H
