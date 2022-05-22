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

