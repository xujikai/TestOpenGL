//
// Created by Administrator on 2022/8/16.
//

#ifndef TESTOPENGL_BEATHEARTSAMPLE_H
#define TESTOPENGL_BEATHEARTSAMPLE_H

#include <gtc/matrix_transform.hpp>
#include "./GLSampleBase.h"
#include "../util/LogUtil.h"

class BeatHeartSample : public GLSampleBase {
public:
    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

    void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

private:
    GLuint aPositionLoc = GL_NONE;
    GLint uMvpMatrixLoc = GL_NONE;
    GLint uTimeLoc = GL_NONE;
    GLint uScreenSizeLoc = GL_NONE;

    GLuint mVaoIds[1] = {GL_NONE};
    GLuint mVboIds[2] = {GL_NONE};

    glm::mat4 mMvpMatrix = glm::mat4(1.0);

    int mRotateX = 0;
    int mRotateY = 0;
};


#endif //TESTOPENGL_BEATHEARTSAMPLE_H
