package com.app.testopengl.opengl

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyCameraRender: GLSurfaceView.Renderer {

    var mNativeRender: MyNativeCameraRender = MyNativeCameraRender()
    lateinit var mGLSurfaceView: GLSurfaceView

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig?) {
        mNativeRender.nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        mNativeRender.nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mNativeRender.nativeOnDrawFrame()
    }

    fun init(surfaceView: GLSurfaceView) {
        mGLSurfaceView = surfaceView
        mGLSurfaceView.setEGLContextClientVersion(2)
        mGLSurfaceView.setRenderer(this)
        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY

        mNativeRender.nativeCreateContext()
        mNativeRender.nativeInit()
    }

    fun unInit() {
        mNativeRender.nativeUnInit()
        mNativeRender.nativeDestroyContext()
    }

    fun setRenderFrame(format: Int, width: Int, height: Int, data: ByteArray) {
        mNativeRender.nativeUpdateFrame(format, width, height, data)
    }

    fun requestRender() {
        mGLSurfaceView.requestRender()
    }

    fun setTransformMatrix(translateX: Float, translateY: Float, scaleX: Float, scaleY: Float, degree: Int, mirror: Int) {
        mNativeRender.nativeSetTransformMatrix(translateX, translateY, scaleX, scaleY, degree, mirror)
    }
}