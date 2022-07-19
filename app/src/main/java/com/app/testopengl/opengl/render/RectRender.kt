package com.app.testopengl.opengl.render

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class RectRender: GLSurfaceView.Renderer {

    private var mProgram: Int = -1

    var mVertexArr = floatArrayOf(
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f
    )
    lateinit var mVertexBuffer: FloatBuffer

    private var aPositionLocation = -1
    private var uProjectionMatrixLocation = -1
    private var uColorLocation = -1

    private var mProjectionMatrix = FloatArray(16)

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram = GLComUtils.createProgram(R.raw.rect_vert_shader, R.raw.rect_frag_shader)

        aPositionLocation = GLES20.glGetAttribLocation(mProgram, "aPosition")
        uProjectionMatrixLocation = GLES20.glGetUniformLocation(mProgram, "uProjectionMatrix")
        uColorLocation = GLES20.glGetUniformLocation(mProgram, "uColor")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)

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

        GLES20.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, mProjectionMatrix, 0)

        GLES20.glUniform4f(uColorLocation, 0f, 0f, 1f, 1f)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6)

        GLES20.glDisableVertexAttribArray(aPositionLocation)
        GLES20.glUseProgram(GLES20.GL_NONE)
    }
}