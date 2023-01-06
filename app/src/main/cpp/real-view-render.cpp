#include <jni.h>
#include "render/RealViewRender.h"

//
// Created by Administrator on 2023/1/6.
//

extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeInit(JNIEnv *env, jobject thiz) {
    RealViewRender::GetInstance()->Init();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeUnInit(JNIEnv *env, jobject thiz) {
    RealViewRender::GetInstance()->UnInit();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeOnSurfaceCreated(JNIEnv *env, jobject thiz) {
    RealViewRender::GetInstance()->OnSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                     jint width, jint height) {
    RealViewRender::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeOnDrawFrame(JNIEnv *env, jobject thiz) {
    RealViewRender::GetInstance()->OnDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeSetImageData(JNIEnv *env, jobject thiz,
                                                                 jint format, jint width,
                                                                 jint height, jbyteArray bytes) {
    int len = env->GetArrayLength(bytes);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte *>(buf));
    RealViewRender::GetInstance()->SetImageData(format, width, height, buf);
    delete[] buf;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeSetObjImageData(JNIEnv *env, jobject thiz,
                                                                    jint format, jint width,
                                                                    jint height, jbyteArray bytes) {
    int len = env->GetArrayLength(bytes);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte *>(buf));
    RealViewRender::GetInstance()->SetObjImageData(format, width, height, buf);
    delete[] buf;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_RealViewRender_nativeSetTransformMatrix(JNIEnv *env, jobject thiz,
                                                                       jfloat translate_x,
                                                                       jfloat translate_y,
                                                                       jfloat scale_x,
                                                                       jfloat scale_y, jint degree,
                                                                       jint mirror) {
    RealViewRender::GetInstance()->SetTransformMatrix(translate_x, translate_y, scale_x, scale_y, degree, mirror);
}