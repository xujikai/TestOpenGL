package com.app.testopengl.opengl

class MyNativeCameraRender {

    companion object {
        init {
            System.loadLibrary("native-render")
        }
    }

    external fun nativeCreateContext()

    external fun nativeDestroyContext()

    external fun nativeInit()

    external fun nativeUnInit()

    external fun nativeOnSurfaceCreated()

    external fun nativeOnSurfaceChanged(width: Int, height: Int)

    external fun nativeOnDrawFrame()

    external fun nativeUpdateFrame(format: Int, width: Int, height: Int, data: ByteArray)

    external fun nativeSetTransformMatrix(translateX: Float, translateY: Float, scaleX: Float, scaleY: Float, degree: Int, mirror: Int)

}