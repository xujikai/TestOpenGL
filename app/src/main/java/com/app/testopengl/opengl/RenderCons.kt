package com.app.testopengl.opengl

object RenderCons {

    const val GL_RENDER_TYPE = 0
    const val CL_RENDER_TYPE = 1

    const val IMAGE_FORMAT_RGBA = 0x01
    const val IMAGE_FORMAT_NV21 = 0x02
    const val IMAGE_FORMAT_NV12 = 0x03
    const val IMAGE_FORMAT_I420 = 0x04

    const val PARAM_TYPE_SET_SHADER_INDEX = 201
    const val PARAM_TYPE_SET_EXAMPLE = 202

    const val EXAMPLE_TYPE = 300
    const val EXAMPLE_TYPE_KEY_CONVEYOR_BELT = EXAMPLE_TYPE
    const val EXAMPLE_TYPE_KEY_BLUELINE_CHALLENGE = EXAMPLE_TYPE + 1

    const val RECORDER_TYPE_SINGLE_VIDEO = 0 // 仅录制视频
    const val RECORDER_TYPE_SINGLE_AUDIO = 1 // 仅录制音频
    const val RECORDER_TYPE_AV = 2 // 同时录制音频和视频,打包成 MP4 文件

}