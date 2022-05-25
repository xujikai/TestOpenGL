//
// Created by 徐极凯 on 2022/5/21.
//

#include "MyMediaRecorderContext.h"
#include "MediaCameraRender.h"

MyMediaRecorderContext *MyMediaRecorderContext::m_pContext = nullptr;

MyMediaRecorderContext::MyMediaRecorderContext() {
    MediaCameraRender::GetInstance();
}

MyMediaRecorderContext::~MyMediaRecorderContext() {
    MediaCameraRender::DestroyInstance();
}

void MyMediaRecorderContext::Init() {
    MediaCameraRender::GetInstance()->Init(0, 0, nullptr);
    MediaCameraRender::GetInstance()->SetRenderCallback(this, OnGLRenderFrame);
}

void MyMediaRecorderContext::UnInit() {
    MediaCameraRender::GetInstance()->UnInit();
}

void MyMediaRecorderContext::OnSurfaceCreated() {
    MediaCameraRender::GetInstance()->OnSurfaceCreated();
}

void MyMediaRecorderContext::OnSurfaceChanged(int width, int height) {
    MediaCameraRender::GetInstance()->OnSurfaceChanged(width, height);
}

void MyMediaRecorderContext::OnDrawFrame() {
    MediaCameraRender::GetInstance()->OnDrawFrame();
}

void MyMediaRecorderContext::UpdateFrame(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
        case IMAGE_FORMAT_RGBA:
            nativeImage.pLineSize[0] = width * 4;
            break;
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.pLineSize[0] = width;
            nativeImage.pLineSize[1] = width;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            nativeImage.pLineSize[0] = width;
            nativeImage.pLineSize[1] = width / 2;
            nativeImage.pLineSize[2] = width / 2;
            break;
        default:
            break;
    }

    MediaCameraRender::GetInstance()->RenderVideoFrame(&nativeImage);
}

void MyMediaRecorderContext::OnAudioData(uint8_t *pData, int size) {
    AudioFrame audioFrame(pData, size, false);
    LOGCATE("MyMediaRecorderContext::AV::OnFrame2EncodeAudio");
    if(m_pAVRecorder != nullptr)
        m_pAVRecorder->OnFrame2Encode(&audioFrame);
}

void MyMediaRecorderContext::SetTransformMatrix(float translateX, float translateY, float scaleX,
                                                float scaleY, int degree, int mirror) {
    m_TransformMatrix.translateX = translateX;
    m_TransformMatrix.translateY = translateY;
    m_TransformMatrix.scaleX = scaleX;
    m_TransformMatrix.scaleY = scaleY;
    m_TransformMatrix.degree = degree;
    m_TransformMatrix.mirror = mirror;
    MediaCameraRender::GetInstance()->UpdateMVPMatrix(&m_TransformMatrix);
}

int MyMediaRecorderContext::StartRecord(int recorderType, const char *outUrl, int frameWidth,
                                        int frameHeight, long videoBitRate, int fps) {
    std::unique_lock<std::mutex> lock(m_Mutex);
    switch (recorderType) {
        case RECORDER_TYPE_SINGLE_VIDEO:
            if (m_pVideoRecorder == nullptr) {
                m_pVideoRecorder = new SingleVideoRecorder(outUrl, frameWidth, frameHeight, videoBitRate, fps);
                m_pVideoRecorder->StartRecord();
            }
            break;
        case RECORDER_TYPE_AV:
            if(m_pAVRecorder == nullptr) {
                RecorderParam param = {0};
                param.frameWidth      = frameWidth;
                param.frameHeight     = frameHeight;
                param.videoBitRate    = videoBitRate;
                param.fps             = fps;
                param.audioSampleRate = DEFAULT_SAMPLE_RATE;
                param.channelLayout   = AV_CH_LAYOUT_STEREO;
                param.sampleFormat    = AV_SAMPLE_FMT_S16;
                m_pAVRecorder = new MediaRecorder(outUrl, &param);
                m_pAVRecorder->StartRecord();
            }
            break;
        default:
            break;
    }
    return 0;
}

int MyMediaRecorderContext::StopRecord() {
    std::unique_lock<std::mutex> lock(m_Mutex);
    if (m_pVideoRecorder != nullptr) {
        m_pVideoRecorder->StopRecord();
        delete m_pVideoRecorder;
        m_pVideoRecorder = nullptr;
    }
    if(m_pAVRecorder != nullptr) {
        m_pAVRecorder->StopRecord();
        delete m_pAVRecorder;
        m_pAVRecorder = nullptr;
    }
    return 0;
}

void MyMediaRecorderContext::OnGLRenderFrame(void *ctx, NativeImage *pImage) {
    LOGCATE("MyMediaRecorderContext::OnGLRenderFrame ctx=%p, pImage=%p", ctx, pImage);
    LOGCATE("MyMediaRecorderContext::OnGLRenderFrame src[w,h,format]=[%d, %d, %d]", pImage->width, pImage->height, pImage->format);
    LOGCATE("MyMediaRecorderContext::OnGLRenderFrame src[line0,line1,line2]=[%d, %d, %d]", pImage->pLineSize[0], pImage->pLineSize[1], pImage->pLineSize[2]);
    MyMediaRecorderContext *context = static_cast<MyMediaRecorderContext *>(ctx);
    std::unique_lock<std::mutex> lock(context->m_Mutex);
    if (context->m_pVideoRecorder != nullptr) {
        LOGCATE("MyMediaRecorderContext::Video::OnFrame2Encode");
        context->m_pVideoRecorder->OnFrame2Encode(pImage);
    }
    if(context->m_pAVRecorder != nullptr) {
        LOGCATE("MyMediaRecorderContext::AV::OnFrame2EncodeVideo");
        context->m_pAVRecorder->OnFrame2Encode(pImage);
    }
}

MyMediaRecorderContext * MyMediaRecorderContext::GetInstance() {
    if (m_pContext == nullptr) {
        m_pContext = new MyMediaRecorderContext();
    }
    return m_pContext;
}

void MyMediaRecorderContext::DestroyInstance() {
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }
}

