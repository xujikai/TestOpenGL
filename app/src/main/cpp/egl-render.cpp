#include <jni.h>
#include "render/MyEGLRenderContext.h"

//
// Created by 徐极凯 on 2022/4/23.
//

extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeEglRender_nativeEglInit(JNIEnv *env, jobject thiz) {
    MyEGLRenderContext::GetInstance()->Init();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeEglRender_nativeEglDraw(JNIEnv *env, jobject thiz) {
    MyEGLRenderContext::GetInstance()->Draw();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeEglRender_nativeEglUnInit(JNIEnv *env, jobject thiz) {
    MyEGLRenderContext::GetInstance()->UnInit();
    MyEGLRenderContext::DestroyInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeEglRender_nativeEglSetImageData(JNIEnv *env, jobject thiz,
                                                                       jint width, jint height,
                                                                       jbyteArray bytes) {
    int len = env->GetArrayLength(bytes);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte *>(buf));
    MyEGLRenderContext::GetInstance()->SetImageData(width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(bytes);
}