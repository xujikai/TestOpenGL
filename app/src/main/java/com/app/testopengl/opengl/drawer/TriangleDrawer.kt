package com.app.testopengl.opengl.drawer

import android.content.Context
import android.opengl.GLES20
import com.app.testopengl.R
import com.app.testopengl.opengl.BaseDrawer
import com.app.testopengl.utils.ShaderHelper
import com.app.testopengl.utils.VertexArray
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TriangleDrawer(context: Context): BaseDrawer(context) {

    private val A_POSITION = "a_Position"
    private val U_COLOR = "u_Color"

    private var uColorLocation = -1
    private var aPositionLocation = -1

    var triangleVertex = floatArrayOf(
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f
    )

    init {
        mProgram = ShaderHelper.buildProgram(context, R.raw.triangle_vert_shader, R.raw.triangle_frag_shader)

        GLES20.glUseProgram(mProgram)
        vertexArray = VertexArray(triangleVertex)
        POSITION_COMPONENT_COUNT = 2
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        aPositionLocation = GLES20.glGetAttribLocation(mProgram, A_POSITION)
        uColorLocation = GLES20.glGetUniformLocation(mProgram, U_COLOR)

        vertexArray.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0)
    }

    override fun onDrawFrame(gl: GL10) {
        super.onDrawFrame(gl)

        GLES20.glUniform4f(uColorLocation, 0f, 0f, 1f, 1f)
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3)
    }

}