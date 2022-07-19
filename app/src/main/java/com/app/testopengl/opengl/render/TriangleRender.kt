package com.app.testopengl.opengl.render

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TriangleRender: GLSurfaceView.Renderer {

    private var mProgram: Int = -1

    private var mVertexArr = floatArrayOf(
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    )
    private lateinit var mVertxBuffer: FloatBuffer

    private var aPositionLocation = -1
    private var uColorLocation = -1

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram = GLComUtils.createProgram(R.raw.triangle_vert_shader, R.raw.triangle_frag_shader)

        aPositionLocation = GLES20.glGetAttribLocation(mProgram, "a_Position")
        uColorLocation = GLES20.glGetUniformLocation(mProgram, "u_Color")

        mVertxBuffer = GLComUtils.createFloatBuffer(mVertexArr)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        GLES20.glClearColor(0f, 0f, 0f, 1f)
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)

        GLES20.glUseProgram(mProgram)

        GLES20.glEnableVertexAttribArray(aPositionLocation)
        GLES20.glVertexAttribPointer(aPositionLocation, 2, GLES20.GL_FLOAT, false, 0, mVertxBuffer)

        GLES20.glUniform4f(uColorLocation, 0f, 0f, 1f, 1f)

        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3)

        GLES20.glDisableVertexAttribArray(aPositionLocation)
        GLES20.glUseProgram(GLES20.GL_NONE)
    }
}