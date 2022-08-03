//
// Created by 徐极凯 on 2022/4/16.
//

#ifndef TESTOPENGL_TRIANGLESAMPLE_H
#define TESTOPENGL_TRIANGLESAMPLE_H

#include <GLES3/gl3.h>
#include "GLSampleBase.h"

class TriangleSample: public GLSampleBase {

public:
    TriangleSample();

    ~TriangleSample();

    virtual void Init();

    virtual void Draw(int screenW, int screenH);

    virtual void UnInit();

private:
    int aPositionLocation;
};

#endif //TESTOPENGL_TRIANGLESAMPLE_H
