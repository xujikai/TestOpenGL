package com.app.testopengl.opengl.drawer

import android.opengl.GLES20
import com.app.testopengl.opengl.OpenGLTools
import java.nio.FloatBuffer

class TriangleDrawer: IDrawer {

    private val mVertexCoors = floatArrayOf(
        -1f, -1f,
        1f, -1f,
        0f, 1f
    )

//    private val mTextureCoors = floatArrayOf(
//        0f, 1f,
//        1f, 1f,
//        0.5f, 0f
//    )

    private lateinit var mVertexBuffer: FloatBuffer
//    private lateinit var mTextureBuffer: FloatBuffer

    private var mTextureId: Int = -1
    private var mProgram: Int = -1

    private var mVertexPosHandler: Int = -1
//    private var mTexturePosHandler: Int = -1

    init {
        // 步骤1：初始化顶点坐标
        initPos()
    }

    private fun initPos() {
        mVertexBuffer = OpenGLTools.getFloatBuffer(mVertexCoors)
//        mTextureBuffer = OpenGLTools.getFloatBuffer(mTextureCoors)
    }

    override fun setTextureId(id: Int) {
        mTextureId = id
    }

    override fun draw() {
        if (mTextureId != -1) {
            // 步骤2：创建、编译并启动 OpenGL 着色器
            createGLProgram()
            // 步骤3：开始渲染绘制
            doDraw()
        }
    }

    private fun createGLProgram() {
        if (mProgram == -1) {
            mProgram = OpenGLTools.initProgramByAssets(
                "vshader/triangle.glsl", "fshader/triangle.glsl"
            )

            mVertexPosHandler = GLES20.glGetAttribLocation(mProgram, "aPosition")
//            mTexturePosHandler = GLES20.glGetAttribLocation(mProgram, "aCoordinate")
        }
        GLES20.glUseProgram(mProgram)
    }

    private fun doDraw() {
        GLES20.glEnableVertexAttribArray(mVertexPosHandler)
//        GLES20.glEnableVertexAttribArray(mTexturePosHandler)

        GLES20.glVertexAttribPointer(mVertexPosHandler, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
//        GLES20.glVertexAttribPointer(mTexturePosHandler, 2, GLES20.GL_FLOAT, false, 0, mTextureBuffer)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 3)
    }

    override fun release() {
        GLES20.glDisableVertexAttribArray(mVertexPosHandler)
//        GLES20.glDisableVertexAttribArray(mTexturePosHandler)

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
        GLES20.glDeleteTextures(1, intArrayOf(mTextureId), 0)
        GLES20.glDeleteProgram(mProgram)
    }

}