package com.app.testopengl.opengl

class MyNativeRender {

    companion object {
        init {
            System.loadLibrary("native-render")
        }
    }

    external fun nativeInit()

    external fun nativeUnInit()

    external fun nativeOnSurfaceCreated()

    external fun nativeOnSurfaceChanged(width: Int, height: Int)

    external fun nativeOnDrawFrame()

    external fun nativeSetImageData(format: Int, width: Int, height: Int, bytes: ByteArray)

}