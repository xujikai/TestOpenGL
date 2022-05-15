package com.app.testopengl.opengl

class MyNativeEglRender {

    companion object {
        init {
            System.loadLibrary("native-render")
        }
    }

    external fun nativeEglInit()

    external fun nativeEglDraw()

    external fun nativeEglUnInit()

    external fun nativeEglSetImageData(width: Int, height: Int, bytes: ByteArray)

}