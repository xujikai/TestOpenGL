#include <jni.h>
#include <string>
#include "render/MyGLRenderContext.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeInit(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeUnInit(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::DestroyInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeOnSurfaceCreated(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                     jint width, jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeOnDrawFrame(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeSetImageData(JNIEnv *env, jobject thiz,
                                                                 jint format, jint width,
                                                                 jint height, jbyteArray imageData) {
    int len = env->GetArrayLength(imageData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
    MyGLRenderContext::GetInstance()->SetImageData(format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_opengl_MyGLRender_nativeSetImageDataWithIndex(JNIEnv *env, jobject thiz,
                                                                      jint index, jint format,
                                                                      jint width, jint height,
                                                                      jbyteArray imageData) {
    int len = env->GetArrayLength(imageData);
    uint8_t *buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
    MyGLRenderContext::GetInstance()->SetImageDataWithIndex(index, format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}