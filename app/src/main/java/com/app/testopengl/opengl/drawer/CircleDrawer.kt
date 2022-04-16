package com.app.testopengl.opengl.drawer

import android.content.Context
import android.opengl.GLES20
import com.app.testopengl.R
import com.app.testopengl.opengl.BaseDrawer
import com.app.testopengl.utils.ShaderHelper
import com.app.testopengl.utils.VertexArray
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.cos
import kotlin.math.sin

class CircleDrawer(context: Context): BaseDrawer(context) {

    private val A_POSITION = "a_Position"
    private val U_COLOR = "u_Color"

    private var uColorLocation = -1
    private var aPositionLocation = -1

    val circleVertexNum = 360
    val radius = 0.8f
    var circleVertex: FloatArray = FloatArray((circleVertexNum + 2) * 2)

    init {
        // 中心点
        circleVertex[0] = 0f
        circleVertex[1] = 0f
        // 循环点
        var radians: Double
        for (i in 0 until circleVertexNum) {
            radians = Math.toRadians(i.toDouble())
            circleVertex[2 * i + 2] = (cos(radians) * radius).toFloat()
            circleVertex[2 * i + 3] = (sin(radians) * radius).toFloat()
        }
        // 闭合点
        radians = Math.toRadians(0.0)
        circleVertex[circleVertex.size - 2] = (cos(radians) * radius).toFloat()
        circleVertex[circleVertex.size - 1] = (sin(radians) * radius).toFloat()
    }

    init {
        mProgram = ShaderHelper.buildProgram(context, R.raw.triangle_vert_shader, R.raw.triangle_frag_shader)

        GLES20.glUseProgram(mProgram)
        vertexArray = VertexArray(circleVertex)
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
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, circleVertexNum + 2)
    }

}