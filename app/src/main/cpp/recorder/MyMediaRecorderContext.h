//
// Created by 徐极凯 on 2022/5/21.
//

#ifndef TESTOPENGL_MYMEDIARECORDERCONTEXT_H
#define TESTOPENGL_MYMEDIARECORDERCONTEXT_H

#include <thread>
#include "../util/ImageDef.h"
#include "../util/ByteFlowDef.h"
#include "SingleVideoRecorder.h"
#include "MediaRecorder.h"

#define RECORDER_TYPE_SINGLE_VIDEO  0 //仅录制视频
#define RECORDER_TYPE_SINGLE_AUDIO  1 //仅录制音频
#define RECORDER_TYPE_AV            2 //同时录制音频和视频,打包成 MP4 文件

#define DEFAULT_SAMPLE_RATE    44100

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

    void OnAudioData(uint8_t *pData, int size);

    void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

    int StartRecord(int recorderType, const char* outUrl, int frameWidth, int frameHeight, long videoBitRate, int fps);

    int StopRecord();

    static void OnGLRenderFrame(void *ctx, NativeImage * pImage);

    static MyMediaRecorderContext* GetInstance();

    static void DestroyInstance();

private:
    static MyMediaRecorderContext *m_pContext;
    TransformMatrix m_TransformMatrix;
    SingleVideoRecorder *m_pVideoRecorder = nullptr;
    MediaRecorder       *m_pAVRecorder    = nullptr;
    std::mutex m_Mutex;
};


#endif //TESTOPENGL_MYMEDIARECORDERCONTEXT_H
