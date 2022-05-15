//
// Created by 徐极凯 on 2022/4/16.
//

#include "MyGLRenderContext.h"
#include "../sample/TriangleSample.h"
#include "../sample/TextureMapSample.h"
#include "../sample/NV21TextureMapSample.h"
#include "../sample/VaoSample.h"
#include "../sample/FBOSample.h"
#include "../sample/CoordSystemSample.h"
#include <GLES3/gl3.h>

MyGLRenderContext* MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext() {
//    m_pSample = new TriangleSample();
//    m_pSample = new TextureMapSample();
//    m_pSample = new NV21TextureMapSample();
//    m_pSample = new VaoSample();
//    m_pSample = new FBOSample();
    m_pSample = new CoordSystemSample();
}

MyGLRenderContext::~MyGLRenderContext() {
    if (m_pSample) {
        delete m_pSample;
        m_pSample = nullptr;
    }
}

void MyGLRenderContext::OnSurfaceCreated() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void MyGLRenderContext::OnDrawFrame() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    if (m_pSample) {
        m_pSample->Init();
        m_pSample->Draw(m_ScreenW, m_ScreenH);
    }
}

void MyGLRenderContext::SetImageData(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.format = format;
    nativeImage.ppPlane[0] = pData;

    switch (format)
    {
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

    if (m_pSample) {
        m_pSample->LoadImage(&nativeImage);
    }
}

MyGLRenderContext * MyGLRenderContext::GetInstance() {
    if (m_pContext == nullptr) {
        m_pContext = new MyGLRenderContext();
    }
    return m_pContext;
}

void MyGLRenderContext::DestroyInstance() {
    if (m_pContext) {
        delete m_pContext;
        m_pContext = nullptr;
    }
}
