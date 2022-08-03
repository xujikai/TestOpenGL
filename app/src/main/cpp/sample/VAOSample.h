//
// Created by 徐极凯 on 2022/4/17.
//

#ifndef TESTOPENGL_VAOSAMPLE_H
#define TESTOPENGL_VAOSAMPLE_H

#include "GLSampleBase.h"

class VAOSample: public GLSampleBase {

public:

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void UnInit() override;

private:
    GLuint aPositionLoc;
    GLuint aColorLoc;

    GLuint mVaoId;
    GLuint mVboIds[2];

};


#endif //TESTOPENGL_VAOSAMPLE_H
