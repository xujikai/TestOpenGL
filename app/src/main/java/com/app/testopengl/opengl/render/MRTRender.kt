package com.app.testopengl.opengl.render

import android.opengl.GLES30
import android.opengl.GLSurfaceView
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import com.app.testopengl.utils.GLTexUtils
import com.blankj.utilcode.util.LogUtils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MRTRender: GLSurfaceView.Renderer {

    private var mProgramMrt = -1
    private var mProgram = -1

    private var uTextureMrtLocation = -1
    private var uTextureLocation = -1

    private val mVertexArr = floatArrayOf(-1f, -1f, -1f, 1f, 1f, 1f, -1f, -1f, 1f, 1f, 1f, -1f)
    private val mTexture0Arr = floatArrayOf(0f, 1f, 0f, 0f, 1f, 0f, 0f, 1f, 1f, 0f, 1f, 1f)
    private val mTexture1Arr = floatArrayOf(0f, 0f, 0f, 1f, 1f, 1f, 0f, 0f, 1f, 1f, 1f, 0f)
    private lateinit var mVertexBuffer: FloatBuffer
    private lateinit var mTexture0Buffer: FloatBuffer
    private lateinit var mTexture1Buffer: FloatBuffer

    private val mVertexLeftBottomArr = floatArrayOf(-1f, -1f, -1f, 0f, 0f, 0f, -1f, -1f, 0f, 0f, 0f, -1f)
    private lateinit var mVertexOffsetArr: FloatArray

    private var mImageTexture = -1
    private var mFbo = -1
    private lateinit var mFboTextureArr: IntArray

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgramMrt = GLComUtils.createProgram(R.raw.mrt_vert_shader, R.raw.mrt_frag_three_shader)
        mProgram = GLComUtils.createProgram(R.raw.mrt_vert_shader, R.raw.mrt_frag_screen_shader)

        uTextureMrtLocation = GLES30.glGetUniformLocation(mProgramMrt, "uTexture")
        uTextureLocation = GLES30.glGetUniformLocation(mProgram, "uTexture")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)
        mTexture0Buffer = GLComUtils.createFloatBuffer(mTexture0Arr)
        mTexture1Buffer = GLComUtils.createFloatBuffer(mTexture1Arr)

        mVertexOffsetArr = FloatArray(mVertexLeftBottomArr.size)

        mImageTexture = GLTexUtils.loadTexture(R.drawable.texture)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
        initFrameBuffer(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, mFbo)

//        离屏渲染的窗口大小
//        GLES30.glViewport(0, 0, mImageWidth, mImageHeight);
        GLES30.glClearColor(1f, 1f, 1f,1f)
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)
//        确定绘制到哪些 GLES30.GL_COLOR_ATTACHMENT 上
//        GLES30.glDrawBuffers(attachmentArr.size, attachmentBuffer)
        GLES30.glUseProgram(mProgramMrt)

        GLES30.glEnableVertexAttribArray(0)
        GLES30.glVertexAttribPointer(0, 2, GLES30.GL_FLOAT, false, 0, mVertexBuffer)
        GLES30.glEnableVertexAttribArray(1)
        GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 0, mTexture0Buffer)

        GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mImageTexture)
        GLES30.glUniform1i(uTextureMrtLocation, 0)

        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 6)

        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, 0)

//        屏幕的窗口大小
//        GLES30.glViewport(0, 0, mScreenWidth, mScreenHeight);
        GLES30.glClearColor(1f, 1f, 1f,1f)
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)
        GLES30.glUseProgram(mProgram)

        // 将三个纹理分别绘制出来
        for (i in mFboTextureArr.indices) {
            for (j in mVertexLeftBottomArr.indices) {
                mVertexOffsetArr[j] = mVertexLeftBottomArr[j] + 0.5f * i
            }
            val offsetBuffer = GLComUtils.createFloatBuffer(mVertexOffsetArr)

            GLES30.glEnableVertexAttribArray(0)
            GLES30.glVertexAttribPointer(0, 2, GLES30.GL_FLOAT, false, 0, offsetBuffer)
            GLES30.glEnableVertexAttribArray(1)
            GLES30.glVertexAttribPointer(1, 2, GLES30.GL_FLOAT, false, 0, mTexture1Buffer)

            GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mFboTextureArr[i])
            GLES30.glUniform1i(uTextureLocation, 0)

            GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, 6)
        }
    }

    private fun initFrameBuffer(width: Int, height: Int) {
        val frameBuffers = IntArray(1)
        GLES30.glGenFramebuffers(frameBuffers.size, frameBuffers, 0)
        mFbo = frameBuffers[0]

        mFboTextureArr = IntArray(3)
        GLES30.glGenTextures(mFboTextureArr.size, mFboTextureArr, 0)

        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, mFbo)
        mFboTextureArr.forEachIndexed { index, texture ->
            LogUtils.i(texture)
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texture)
            GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR)
            GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR)
            GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE)
            GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE)
            GLES30.glTexImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGBA, width, height, 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, null)
            GLES30.glFramebufferTexture2D(GLES30.GL_FRAMEBUFFER, GLES30.GL_COLOR_ATTACHMENT0 + index, GLES30.GL_TEXTURE_2D, texture, 0)
            GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, GLES30.GL_NONE)
        }

        val attachmentArr = intArrayOf(GLES30.GL_COLOR_ATTACHMENT0, GLES30.GL_COLOR_ATTACHMENT1, GLES30.GL_COLOR_ATTACHMENT2)
        val attachmentBuffer = GLComUtils.createIntBuffer(attachmentArr)
        GLES30.glDrawBuffers(attachmentArr.size, attachmentBuffer)
        GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, GLES30.GL_NONE)
    }

}