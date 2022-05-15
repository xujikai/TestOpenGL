//
// Created by 徐极凯 on 2022/4/17.
//

#ifndef TESTOPENGL_VAOSAMPLE_H
#define TESTOPENGL_VAOSAMPLE_H

#include "GLSampleBase.h"

class VaoSample: public GLSampleBase {

public:
    VaoSample();

    void Init() override;

    void Draw(int screenW, int screenH) override;

    void Destroy() override;

private:
    GLuint m_VaoId;
    GLuint m_VboIds[2];

};


#endif //TESTOPENGL_VAOSAMPLE_H
