#include <jni.h>
#include <cstring>
#include <libavcodec/version.h>
#include "recorder/MyMediaRecorderContext.h"

//
// Created by 徐极凯 on 2022/5/21.
//


extern "C"
JNIEXPORT jstring JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMedia_nativeGetFFmpegVersion(JNIEnv *env, jobject thiz) {
    char strBuf[1024] = {0};
    strcat(strBuf, "libavcodec : ");
    strcat(strBuf, AV_STRINGIFY(LIBAVCODEC_VERSION));

    return env->NewStringUTF(strBuf);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeCreateContext(JNIEnv *env,jobject thiz) {
    MyMediaRecorderContext::GetInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeDestroyContext(JNIEnv *env,
                                                                          jobject thiz) {
    MyMediaRecorderContext::DestroyInstance();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeInit(JNIEnv *env, jobject thiz) {
    MyMediaRecorderContext::GetInstance()->Init();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeUnInit(JNIEnv *env, jobject thiz) {
    MyMediaRecorderContext::GetInstance()->UnInit();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeOnSurfaceCreated(JNIEnv *env,
                                                                            jobject thiz) {
    MyMediaRecorderContext::GetInstance()->OnSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeOnSurfaceChanged(JNIEnv *env,
                                                                            jobject thiz,
                                                                            jint width,
                                                                            jint height) {
    MyMediaRecorderContext::GetInstance()->OnSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeOnDrawFrame(JNIEnv *env, jobject thiz) {
    MyMediaRecorderContext::GetInstance()->OnDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeUpdateFrame(JNIEnv *env, jobject thiz,
                                                                       jint format, jint width,
                                                                       jint height,
                                                                       jbyteArray data) {
    int len = env->GetArrayLength(data);
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte *>(buf));
    MyMediaRecorderContext::GetInstance()->UpdateFrame(format, width, height, buf);
    delete[] buf;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_nativeSetTransformMatrix(JNIEnv *env,
                                                                              jobject thiz,
                                                                              jfloat translate_x,
                                                                              jfloat translate_y,
                                                                              jfloat scale_x,
                                                                              jfloat scale_y,
                                                                              jint degree,
                                                                              jint mirror) {
    MyMediaRecorderContext::GetInstance()->SetTransformMatrix(translate_x, translate_y, scale_x, scale_y, degree, mirror);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_startRecord(JNIEnv *env, jobject thiz,
                                                                 jint recorder_type,
                                                                 jstring out_url, jint frame_width,
                                                                 jint frame_height,
                                                                 jlong video_bit_rate, jint fps) {
    const char *url = env->GetStringUTFChars(out_url, nullptr);
    MyMediaRecorderContext::GetInstance()->StartRecord(recorder_type, url, frame_width, frame_height, video_bit_rate, fps);
    env->ReleaseStringUTFChars(out_url, url);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_stopRecord(JNIEnv *env, jobject thiz) {
    MyMediaRecorderContext::GetInstance()->StopRecord();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_app_testopengl_ffmpeg_MyNativeMediaRecorder_onAudioData(JNIEnv *env, jobject thiz,
                                                                 jbyteArray data) {
    int len = env->GetArrayLength(data);
    unsigned char *buf = new unsigned char[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte *>(buf));
    MyMediaRecorderContext::GetInstance()->OnAudioData(buf, len);
    delete[] buf;
}