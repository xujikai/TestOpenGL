//
// Created by 徐极凯 on 2022/4/16.
//

#include "MyGLRenderContext.h"
#include "../sample/TriangleSample.h"
#include "../sample/TextureMapSample.h"
#include "../sample/NV21TextureMapSample.h"
#include "../sample/VAOSample.h"
#include "../sample/FBOSample.h"
#include "../sample/CoordSystemSample.h"
#include "../sample/ModelObjSample.h"
#include "../sample/PBOSample.h"
#include "../sample/BeatHeartSample.h"
#include "../sample/BlitSample.h"
#include <GLES3/gl3.h>

MyGLRenderContext* MyGLRenderContext::m_pContext = nullptr;

MyGLRenderContext::MyGLRenderContext() {
//    m_pSample = new TriangleSample();
//    m_pSample = new TextureMapSample();
//    m_pSample = new NV21TextureMapSample();
//    m_pSample = new VAOSample();
//    m_pSample = new FBOSample();
//    m_pSample = new CoordSystemSample();
//    m_pSample = new ModelObjSample();
//    m_pSample = new PBOSample();
//    m_pSample = new BeatHeartSample();
    m_pSample = new BlitSample();
}

MyGLRenderContext::~MyGLRenderContext() {
    if (m_pSample) {
        m_pSample->UnInit();
        delete m_pSample;
        m_pSample = nullptr;
    }
}

void MyGLRenderContext::OnSurfaceCreated() {
    if (m_pSample) {
        m_pSample->Init();
    }
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    m_ScreenW = width;
    m_ScreenH = height;
}

void MyGLRenderContext::OnDrawFrame() {
    if (m_pSample) {
        m_pSample->Draw(m_ScreenW, m_ScreenH);
    }
}

void MyGLRenderContext::SetImageData(int format, int width, int height, uint8_t *pData) {
    NativeImage nativeImage;
    nativeImage.width = width;
    nativeImage.height = height;
    nativeImage.format = format;
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

    if (m_pSample) {
        m_pSample->SetImageData(&nativeImage);
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
