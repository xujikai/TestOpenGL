//
// Created by Administrator on 2022/8/5.
//

#ifndef TESTOPENGL_PBOSAMPLE_H
#define TESTOPENGL_PBOSAMPLE_H

#include "GLSampleBase.h"

class PBOSample : public GLSampleBase{
public:
    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void UnInit();
};


#endif //TESTOPENGL_PBOSAMPLE_H
