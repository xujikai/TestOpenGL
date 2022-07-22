package com.app.testopengl.opengl.render

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import com.blankj.utilcode.util.ConvertUtils
import com.blankj.utilcode.util.Utils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class FBONv21Render: GLSurfaceView.Renderer {

    private var mProgram0 = -1
    private var mProgram1 = -1

    private var aPosition0Location = -1
    private var aTextureCoordinates0Location = -1
    private var uYTexture0Location = -1
    private var uUvTexture0Location = -1

    private var aPosition1Location = -1
    private var aTextureCoordinates1Location = -1
    private var uTexture1Location = -1

    private val mVertexArr = floatArrayOf(-1f, -1f, -1f, 1f, 1f, 1f, -1f, -1f, 1f, 1f, 1f, -1f)
    private val mTexture0Arr = floatArrayOf(0f, 1f, 0f, 0f, 1f, 0f, 0f, 1f, 1f, 0f, 1f, 1f)
    private val mTexture1Arr = floatArrayOf(0f, 0f, 0f, 1f, 1f, 1f, 0f, 0f, 1f, 1f, 1f, 0f)
    private lateinit var mVertexBuffer: FloatBuffer
    private lateinit var mTexture0Buffer: FloatBuffer
    private lateinit var mTexture1Buffer: FloatBuffer

    private var mImageYTexture = -1
    private var mImageUvTexture = -1

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram0 = GLComUtils.createProgram(R.raw.fbo_vert_shader, R.raw.fbo_frag_nv21_shader)
        mProgram1 = GLComUtils.createProgram(R.raw.fbo_vert_shader, R.raw.fbo_frag_blue_shader)

        aPosition0Location = GLES20.glGetAttribLocation(mProgram0, "aPosition")
        aTextureCoordinates0Location = GLES20.glGetAttribLocation(mProgram0, "aTextureCoordinates")
        uYTexture0Location = GLES20.glGetUniformLocation(mProgram0, "uYTexture")
        uUvTexture0Location = GLES20.glGetUniformLocation(mProgram0, "uUvTexture")

        aPosition1Location = GLES20.glGetAttribLocation(mProgram1, "aPosition")
        aTextureCoordinates1Location = GLES20.glGetAttribLocation(mProgram1, "aTextureCoordinates")
        uTexture1Location = GLES20.glGetUniformLocation(mProgram1, "uTexture")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)
        mTexture0Buffer = GLComUtils.createFloatBuffer(mTexture0Arr)
        mTexture1Buffer = GLComUtils.createFloatBuffer(mTexture1Arr)

        val textures = IntArray(2)
        GLES20.glGenTextures(textures.size, textures, 0)
        mImageYTexture = textures[0]
        mImageUvTexture = textures[1]

        val mImageWidth = 840
        val mImageHeight = 1074
        val imageBytes = ConvertUtils.inputStream2Bytes(Utils.getApp().assets.open("YUV_Image_840x1074.NV21"))
        val imageYBuffer = GLComUtils.createByteBuffer(imageBytes.copyOfRange(0, mImageWidth * mImageHeight))
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mImageYTexture)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, mImageWidth, mImageHeight, 0,
            GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, imageYBuffer)

        val imageUvBuffer = GLComUtils.createByteBuffer(imageBytes.copyOfRange(mImageWidth * mImageHeight, imageBytes.size))
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mImageUvTexture)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE_ALPHA, mImageWidth / 2, mImageHeight / 2, 0,
            GLES20.GL_LUMINANCE_ALPHA, GLES20.GL_UNSIGNED_BYTE, imageUvBuffer)
    }

    private var mFrameBuffer = -1
    private var mFrameBufferTexture = -1

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        initFrameBuffer(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        GLES20.glClearColor(0f, 0f, 0f, 1f)
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)

        GLES20.glUseProgram(mProgram0)

        GLES20.glEnableVertexAttribArray(aPosition0Location)
        GLES20.glVertexAttribPointer(aPosition0Location, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
        GLES20.glEnableVertexAttribArray(aTextureCoordinates0Location)
        GLES20.glVertexAttribPointer(aTextureCoordinates0Location, 2, GLES20.GL_FLOAT, false, 0, mTexture0Buffer)

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mImageYTexture)
        GLES20.glUniform1i(uYTexture0Location, 0)

        GLES20.glActiveTexture(GLES20.GL_TEXTURE1)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mImageUvTexture)
        GLES20.glUniform1i(uUvTexture0Location, 1)

        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6)

        GLES20.glUseProgram(mProgram1)

        GLES20.glEnableVertexAttribArray(aPosition1Location)
        GLES20.glVertexAttribPointer(aPosition1Location, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
        GLES20.glEnableVertexAttribArray(aTextureCoordinates1Location)
        GLES20.glVertexAttribPointer(aTextureCoordinates1Location, 2, GLES20.GL_FLOAT, false, 0, mTexture1Buffer)

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFrameBufferTexture)
        GLES20.glUniform1i(uTexture1Location, 0)

        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6)
    }

    private fun initFrameBuffer(width: Int, height: Int) {
        val frameBuffers = IntArray(1)
        GLES20.glGenFramebuffers(frameBuffers.size, frameBuffers, 0)
        mFrameBuffer = frameBuffers[0]

        val textures = IntArray(1)
        GLES20.glGenTextures(textures.size, textures, 0)
        mFrameBufferTexture = textures[0]

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFrameBufferTexture)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, width, height, 0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null)
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer)
        GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, mFrameBufferTexture, 0)
    }

}