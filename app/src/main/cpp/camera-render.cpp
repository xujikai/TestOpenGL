#include <jni.h>
#include "render/MyCameraRenderContext.h"

//
// Created by 徐极凯 on 2022/5/15.
//


extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeCreateContext(JNIEnv *env, jobject thiz) {
    MyCameraRenderContext::GetInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeDestroyContext(JNIEnv *env,
                                                                         jobject thiz) {
    MyCameraRenderContext::DestroyInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeInit(JNIEnv *env, jobject thiz) {
    MyCameraRenderContext::GetInstance()->Init();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeUnInit(JNIEnv *env, jobject thiz) {
    MyCameraRenderContext::GetInstance()->UnInit();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeOnSurfaceCreated(JNIEnv *env,
                                                                           jobject thiz) {
    MyCameraRenderContext::GetInstance()->OnSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeOnSurfaceChanged(JNIEnv *env,
                                                                           jobject thiz, jint width,
                                                                           jint height) {
    MyCameraRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeOnDrawFrame(JNIEnv *env, jobject thiz) {
    MyCameraRenderContext::GetInstance()->OnDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeUpdateFrame(JNIEnv *env, jobject thiz,
                                                                      jint format, jint width,
                                                                      jint height,
                                                                      jbyteArray data) {
    int len = env->GetArrayLength(data);
    unsigned char *buf = new unsigned char[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte *>(buf));
    MyCameraRenderContext::GetInstance()->UpdateFrame(format, width, height, buf);
    delete[] buf;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyNativeCameraRender_nativeSetTransformMatrix(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jfloat translate_x,
                                                                             jfloat translate_y,
                                                                             jfloat scale_x,
                                                                             jfloat scale_y,
                                                                             jint degree,
                                                                             jint mirror) {
    MyCameraRenderContext::GetInstance()->SetTransformMatrix(translate_x, translate_y, scale_x, scale_y, degree, mirror);
}