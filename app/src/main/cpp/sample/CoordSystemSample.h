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

    void Destroy() override;

    void LoadImage(NativeImage *pImage) override;

    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

    virtual void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int rotateX, int rotateY, float ratio);

private:
    GLuint m_TextureId;
    GLuint m_VaoId;
    GLuint m_VboIds[3];
    GLint m_SamplerLoc;
    GLint m_MVPMatrixLoc;
    NativeImage m_RenderImage;
    glm::mat4 m_MVPMatrix;

    int m_RotateX;
    int m_RotateY;
    float m_ScaleX;
    float m_ScaleY;
};


#endif //TESTOPENGL_COORDSYSTEMSAMPLE_H
