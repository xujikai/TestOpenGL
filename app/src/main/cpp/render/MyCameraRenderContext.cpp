//
// Created by 徐极凯 on 2022/5/15.
//

#include "MyCameraRenderContext.h"
#include "../sample/Camera01Sample.h"

MyCameraRenderContext *MyCameraRenderContext::m_pContext = nullptr;

MyCameraRenderContext::MyCameraRenderContext() {
    m_pSample = new Camera01Sample();
}

MyCameraRenderContext::~MyCameraRenderContext() {
    if (m_pSample) {
//        delete m_pSample;
        m_pSample = nullptr;
    }
}

void MyCameraRenderContext::Init() {
    m_pSample->Init();
}

void MyCameraRenderContext::UnInit() {
    m_pSample->UnInit();
}

void MyCameraRenderContext::OnSurfaceCreated() {
    m_pSample->OnSurfaceCreated();
}

void MyCameraRenderContext::OnSurfaceChanged(int width, int height) {
    m_pSample->OnSurfaceChanged(width, height);
}

void MyCameraRenderContext::OnDrawFrame() {
    m_pSample->OnDrawFrame();
}

void MyCameraRenderContext::UpdateFrame(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.format = format;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.ppPlane[0] = pData;

    switch (format) {
        case IMAGE_FORMAT_NV12:
        case IMAGE_FORMAT_NV21:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            break;
        case IMAGE_FORMAT_I420:
            nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
            nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
            break;
        default:
            break;
    }

    m_pSample->UpdateFrame(&nativeImage);
}

void MyCameraRenderContext::SetTransformMatrix(float translateX, float translateY, float scaleX,
                                               float scaleY, int degree, int mirror) {
    m_pSample->SetTransformMatrix(translateX, translateY, scaleX, scaleY, degree, mirror);
}

MyCameraRenderContext *MyCameraRenderContext::GetInstance() {
    if (m_pContext == nullptr) {
        m_pContext = new MyCameraRenderContext();
    }
    return m_pContext;
}

void MyCameraRenderContext::DestroyInstance() {
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }
}
