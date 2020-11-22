package com.app.testopengl.opengl.drawer

import android.graphics.Bitmap
import android.opengl.GLES20
import android.opengl.GLUtils
import com.app.testopengl.opengl.OpenGLTools
import java.nio.FloatBuffer

class BitmapDrawer(private var mBitmap: Bitmap) : IDrawer {

    private val mVertexCoors = floatArrayOf(
        -1f, -1f,
        1f, -1f,
        -1f, 1f,
        1f, 1f
    )

    private val mTextureCoors = floatArrayOf(
        0f, 1f,
        1f, 1f,
        0f, 0f,
        1f, 0f
    )

    private lateinit var mVertexBuffer: FloatBuffer
    private lateinit var mTextureBuffer: FloatBuffer

    private var mTextureId: Int = -1
    private var mProgram: Int = -1

    private var mVertexPosHandler: Int = -1
    private var mTexturePosHandler: Int = -1
    private var mTextureHandler: Int = -1

    init {
        initPos()
    }

    private fun initPos() {
        mVertexBuffer = OpenGLTools.getFloatBuffer(mVertexCoors)
        mTextureBuffer = OpenGLTools.getFloatBuffer(mTextureCoors)
    }

    override fun setTextureId(id: Int) {
        mTextureId = id
    }

    override fun draw() {
        if (mTextureId != -1) {
            // 创建、编译、启动 OpenGL 着色器
            createGLProgram()
            // 激活并绑定纹理单元
            activeTexture()
            // 绑定图片到纹理
            bindBitmapToTexture()
            // 渲染绘制
            doDraw()
        }
    }

    private fun createGLProgram() {
        if (mProgram == -1) {
            mProgram = OpenGLTools.initProgramByAssets(
                "vshader/bitmap.glsl", "fshader/bitmap.glsl"
            )

            mVertexPosHandler = GLES20.glGetAttribLocation(mProgram, "aPosition")
            mTexturePosHandler = GLES20.glGetAttribLocation(mProgram, "aCoordinate")
            mTextureHandler = GLES20.glGetUniformLocation(mProgram, "uTexture")
        }
        GLES20.glUseProgram(mProgram)
    }

    private fun activeTexture() {
        // 激活指定的纹理单元
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        // 绑定纹理id到纹理单元
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId)
        // 将激活的纹理单元传递到着色器里面
        GLES20.glUniform1i(mTextureHandler, 0)
        // 配置边缘过渡参数
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR.toFloat())
        GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR.toFloat())
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
    }

    private fun bindBitmapToTexture() {
        if (!mBitmap.isRecycled) {
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, mBitmap, 0)
        }
    }

    private fun doDraw() {
        GLES20.glEnableVertexAttribArray(mVertexPosHandler)
        GLES20.glEnableVertexAttribArray(mTexturePosHandler)

        GLES20.glVertexAttribPointer(mVertexPosHandler, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
        GLES20.glVertexAttribPointer(mTexturePosHandler, 2, GLES20.GL_FLOAT, false, 0, mTextureBuffer)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)
    }

    override fun release() {
//        if (!mBitmap.isRecycled) {
//            mBitmap.recycle()
//        }

        GLES20.glDisableVertexAttribArray(mVertexPosHandler)
        GLES20.glDisableVertexAttribArray(mTexturePosHandler)

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
        GLES20.glDeleteTextures(1, intArrayOf(mTextureId), 0)
        GLES20.glDeleteProgram(mProgram)
    }

}