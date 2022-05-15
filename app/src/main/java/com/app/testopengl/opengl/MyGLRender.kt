package com.app.testopengl.opengl

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyGLRender(private val mNativeRender: MyNativeRender): GLSurfaceView.Renderer {
    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mNativeRender.nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        mNativeRender.nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mNativeRender.nativeOnDrawFrame()
    }

    fun init() {
        mNativeRender.nativeInit()
    }

    fun unInit() {
        mNativeRender.nativeUnInit()
    }

    fun setImageData(format: Int, width: Int, height: Int, bytes: ByteArray) {
        mNativeRender.nativeSetImageData(format, width, height, bytes)
    }
}