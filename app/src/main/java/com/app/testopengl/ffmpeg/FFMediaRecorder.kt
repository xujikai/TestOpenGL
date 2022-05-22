package com.app.testopengl.ffmpeg

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class FFMediaRecorder : GLSurfaceView.Renderer {

    var mNativeMediaRecorder = MyNativeMediaRecorder()
    lateinit var mGLSurfaceView: GLSurfaceView

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mNativeMediaRecorder.nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        mNativeMediaRecorder.nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mNativeMediaRecorder.nativeOnDrawFrame()
    }

    fun init(surfaceView: GLSurfaceView) {
        mGLSurfaceView = surfaceView
        mGLSurfaceView.setEGLContextClientVersion(2)
        mGLSurfaceView.setRenderer(this)
        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

        mNativeMediaRecorder.nativeCreateContext()
        mNativeMediaRecorder.nativeInit()
    }

    fun unInit() {
        mNativeMediaRecorder.nativeUnInit()
        mNativeMediaRecorder.nativeDestroyContext()
    }

    fun onPreviewFrame(format: Int, width: Int, height: Int, data: ByteArray) {
        mNativeMediaRecorder.nativeUpdateFrame(format, width, height, data)
    }

    fun requestRender() {
        mGLSurfaceView.requestRender()
    }

    fun setTransformMatrix(degree: Int, mirror: Int) {
        mNativeMediaRecorder.nativeSetTransformMatrix(0f, 0f, 1f, 1f, degree, mirror)
    }
}