//
// Created by 徐极凯 on 2022/5/21.
//

#ifndef TESTOPENGL_MYMEDIARECORDERCONTEXT_H
#define TESTOPENGL_MYMEDIARECORDERCONTEXT_H

#include "../util/ImageDef.h"
#include "../util/ByteFlowDef.h"

class MyMediaRecorderContext {
    MyMediaRecorderContext();

    ~MyMediaRecorderContext();

public:
    void Init();

    void UnInit();

    void OnSurfaceCreated();

    void OnSurfaceChanged(int width, int height);

    void OnDrawFrame();

    void UpdateFrame(int format, int width, int height, uint8_t *pData);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    static MyMediaRecorderContext* GetInstance();

    static void DestroyInstance();

private:
    static MyMediaRecorderContext *m_pContext;
    TransformMatrix m_TransformMatrix;
};


#endif //TESTOPENGL_MYMEDIARECORDERCONTEXT_H
