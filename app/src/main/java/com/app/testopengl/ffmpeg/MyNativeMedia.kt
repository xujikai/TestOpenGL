package com.app.testopengl.ffmpeg

class MyNativeMedia {

    companion object {
        init {
            System.loadLibrary("native-render")
        }
    }

    external fun nativeGetFFmpegVersion(): String

}