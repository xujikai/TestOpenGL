package com.app.testopengl.opengl

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyGLRender: GLSurfaceView.Renderer {

    companion object {
        init {
            System.loadLibrary("native-render")
        }
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        nativeOnDrawFrame()
    }

    fun init() {
        nativeInit()
    }

    fun unInit() {
        nativeUnInit()
    }

    fun setImageData(format: Int, width: Int, height: Int, bytes: ByteArray) {
        nativeSetImageData(format, width, height, bytes)
    }

    private external fun nativeInit()

    private external fun nativeUnInit()

    private external fun nativeOnSurfaceCreated()

    private external fun nativeOnSurfaceChanged(width: Int, height: Int)

    private external fun nativeOnDrawFrame()

    private external fun nativeSetImageData(format: Int, width: Int, height: Int, bytes: ByteArray)

}