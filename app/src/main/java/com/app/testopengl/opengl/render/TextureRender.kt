package com.app.testopengl.opengl.render

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import com.app.testopengl.utils.GLTexUtils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TextureRender: GLSurfaceView.Renderer {

    private var mProgram = -1

    private var aPositionLocation = -1
    private var aTextureCoordinatesLocation = -1
    private var uModelMatrixLocation = -1
    private var uViewMatrixLocation = -1
    private var uProjectionMatrixLocation = -1
    private var uTextureLocation = -1

    // ABD CDB
    // D C
    // A B
    private val mVertexArr = floatArrayOf(
        -1.0f, -1.0f, // A
        1.0f, -1.0f, // B
        -1.0f, 1.0f, // D
        1.0f, 1.0f // C
    )
    // DCA BAC
    // A B
    // D C
    private val mTextureArr = floatArrayOf(
        0.0f, 1.0f, // D
        1.0f, 1.0f, // C
        0.0f, 0.0f, // A
        1.0f, 0.0f // B
    )

    private lateinit var mVertexBuffer: FloatBuffer
    private lateinit var mTextureBuffer: FloatBuffer

    private var mTextureId = -1

    private var mModelMatrix = FloatArray(16)
    private var mViewMatrix = FloatArray(16)
    private var mProjectionMatrix = FloatArray(16)

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram = GLComUtils.createProgram(R.raw.texture_vert_shader, R.raw.texture_frag_shader)

        aPositionLocation = GLES20.glGetAttribLocation(mProgram, "aPosition")
        aTextureCoordinatesLocation = GLES20.glGetAttribLocation(mProgram, "aTextureCoordinates")
        uModelMatrixLocation = GLES20.glGetUniformLocation(mProgram, "uModelMatrix")
        uViewMatrixLocation = GLES20.glGetUniformLocation(mProgram, "uViewMatrix")
        uProjectionMatrixLocation = GLES20.glGetUniformLocation(mProgram, "uProjectionMatrix")
        uTextureLocation = GLES20.glGetUniformLocation(mProgram, "uTexture")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)
        mTextureBuffer = GLComUtils.createFloatBuffer(mTextureArr)

        mTextureId = GLTexUtils.loadTexture(R.drawable.texture)

        Matrix.setIdentityM(mModelMatrix, 0)
        Matrix.setIdentityM(mViewMatrix, 0)
        Matrix.setIdentityM(mProjectionMatrix, 0)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        if (width > height) {
            val aspect = width.toFloat() / height
            Matrix.orthoM(mProjectionMatrix, 0, -aspect, aspect, -1f, 1f, 0f, 10f)
        } else {
            val aspect = height.toFloat() / width
            Matrix.orthoM(mProjectionMatrix, 0, -1f, 1f, -aspect, aspect, 0f, 10f)
        }
    }

    override fun onDrawFrame(gl: GL10) {
        GLES20.glClearColor(0f, 0f, 0f, 1f)
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)

        GLES20.glUseProgram(mProgram)

        GLES20.glEnableVertexAttribArray(aPositionLocation)
        GLES20.glVertexAttribPointer(aPositionLocation, 2, GLES20.GL_FLOAT, false, 0, mVertexBuffer)
        GLES20.glEnableVertexAttribArray(aTextureCoordinatesLocation)
        GLES20.glVertexAttribPointer(aTextureCoordinatesLocation, 2, GLES20.GL_FLOAT, false, 0, mTextureBuffer)

        GLES20.glUniformMatrix4fv(uModelMatrixLocation, 1, false, mModelMatrix, 0)
        GLES20.glUniformMatrix4fv(uViewMatrixLocation, 1, false, mViewMatrix, 0)
        GLES20.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, mProjectionMatrix, 0)

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId)
        GLES20.glUniform1i(uTextureLocation, 0)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)

        GLES20.glDisableVertexAttribArray(aPositionLocation)
        GLES20.glDisableVertexAttribArray(aTextureCoordinatesLocation)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, GLES20.GL_NONE)
        GLES20.glUseProgram(GLES20.GL_NONE)
    }
}