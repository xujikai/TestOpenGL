package com.app.testopengl.opengl.drawer

import android.graphics.SurfaceTexture
import android.opengl.GLES11Ext
import android.opengl.GLES20
import android.opengl.Matrix
import com.app.testopengl.opengl.OpenGLTools
import java.nio.FloatBuffer

class VideoDrawer : IDrawer {

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
    private var mSurfaceTexture: SurfaceTexture? = null
    private var mSurfaceCallback: ((SurfaceTexture) -> Unit)? = null

    private var mVideoWidth = -1
    private var mVideoHeight = -1
    private var mWorldWidth = -1
    private var mWorldHeight = -1
    private var mWidthRatio = 1f
    private var mHeightRatio = 1f
    private var mMatrix: FloatArray? = null

    private var mVertexPosHandler: Int = -1
    private var mVertexMatrixHandler: Int = -1
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
        mSurfaceTexture = SurfaceTexture(id)
        mSurfaceCallback?.invoke(mSurfaceTexture!!)
    }

    override fun setVideoSize(width: Int, height: Int) {
        mVideoWidth = width
        mVideoHeight = height
    }

    override fun setWorldSize(width: Int, height: Int) {
        mWorldWidth = width
        mWorldHeight = height
    }

    override fun draw() {
        if (mTextureId != -1) {
            initDefMatrix()
            createGLProgram()
            activeTexture()
            updateTexture()
            doDraw()
        }
    }

    private fun initDefMatrix() {
        if (mMatrix != null) return
        if (mVideoWidth != -1 && mVideoHeight != -1 &&
            mWorldWidth != -1 && mWorldHeight != -1) {
            mMatrix = FloatArray(16)
            val prjMatrix = FloatArray(16)
            val originRatio = mVideoWidth / mVideoHeight.toFloat()
            val worldRatio = mWorldWidth / mWorldHeight.toFloat()
            if (mWorldWidth > mWorldHeight) {
                if (originRatio > worldRatio) {
                    mHeightRatio = originRatio / worldRatio
                    Matrix.orthoM(
                        prjMatrix, 0,
                        -mWidthRatio, mWidthRatio,
                        -mHeightRatio, mHeightRatio,
                        3f, 5f
                    )
                } else {// 原始比例小于窗口比例，缩放高度度会导致高度超出，因此，高度以窗口为准，缩放宽度
                    mWidthRatio = worldRatio / originRatio
                    Matrix.orthoM(
                        prjMatrix, 0,
                        -mWidthRatio, mWidthRatio,
                        -mHeightRatio, mHeightRatio,
                        3f, 5f
                    )
                }
            } else {
                if (originRatio > worldRatio) {
                    mHeightRatio = originRatio / worldRatio
                    Matrix.orthoM(
                        prjMatrix, 0,
                        -mWidthRatio, mWidthRatio,
                        -mHeightRatio, mHeightRatio,
                        3f, 5f
                    )
                } else {// 原始比例小于窗口比例，缩放高度会导致高度超出，因此，高度以窗口为准，缩放宽度
                    mWidthRatio = worldRatio / originRatio
                    Matrix.orthoM(
                        prjMatrix, 0,
                        -mWidthRatio, mWidthRatio,
                        -mHeightRatio, mHeightRatio,
                        3f, 5f
                    )
                }
            }

            //设置相机位置
            val viewMatrix = FloatArray(16)
            Matrix.setLookAtM(
                viewMatrix, 0,
                0f, 0f, 5.0f,
                0f, 0f, 0f,
                0f, 1.0f, 0f
            )
            //计算变换矩阵
            Matrix.multiplyMM(mMatrix, 0, prjMatrix, 0, viewMatrix, 0)
        }
    }

    private fun createGLProgram() {
        if (mProgram == -1) {
            mProgram = OpenGLTools.initProgramByAssets(
                "vshader/video.glsl", "fshader/video.glsl"
            )

            mVertexPosHandler = GLES20.glGetAttribLocation(mProgram, "aPosition")
            mVertexMatrixHandler = GLES20.glGetUniformLocation(mProgram, "uMatrix")
            mTexturePosHandler = GLES20.glGetAttribLocation(mProgram, "aCoordinate")
            mTextureHandler = GLES20.glGetUniformLocation(mProgram, "uTexture")
        }
        GLES20.glUseProgram(mProgram)
    }

    private fun activeTexture() {
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTextureId)
        GLES20.glUniform1i(mTextureHandler, 0)
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR.toFloat())
        GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR.toFloat())
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
    }

    private fun updateTexture() {
        mSurfaceTexture?.updateTexImage()
    }

    private fun doDraw() {
        GLES20.glEnableVertexAttribArray(mVertexPosHandler)
        GLES20.glEnableVertexAttribArray(mTexturePosHandler)

        GLES20.glUniformMatrix4fv(mVertexMatrixHandler, 1, false, mMatrix, 0)
        GLES20.glVertexAttribPointer(mVertexPosHandler, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
        GLES20.glVertexAttribPointer(mTexturePosHandler, 2, GLES20.GL_FLOAT, false, 0, mTextureBuffer)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)
    }

    override fun release() {
        GLES20.glDisableVertexAttribArray(mVertexPosHandler)
        GLES20.glDisableVertexAttribArray(mTexturePosHandler)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
        GLES20.glDeleteTextures(1, intArrayOf(mTextureId), 0)
        GLES20.glDeleteProgram(mProgram)
    }

    override fun getSurfaceTexture(cb: (st: SurfaceTexture) -> Unit) {
        mSurfaceCallback = cb
    }

}