package com.app.testopengl.opengl.drawer

import android.content.Context
import android.opengl.GLES20
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.opengl.BaseDrawer
import com.app.testopengl.utils.ShaderHelper
import com.app.testopengl.utils.VertexArray
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class RectDrawer(context: Context): BaseDrawer(context) {

    private val A_POSITION = "aPosition"
    private val U_COLOR = "uColor"

    // 修正图形变形问题，因为物体绘制到平面上，经历了五个坐标空间和三个转换矩阵
    private val U_MODEL_MATRIX = "uModelMatrix"
    private val U_VIEW_MATRIX = "uViewMatrix"
    private val U_PROJECTION_MATRIX = "uProjectionMatrix"

    private var uColorLocation = -1
    private var aPositionLocation = -1

    private var uModelMatrixLocation = -1
    private var uViewMatrixLocation = -1
    private var uProjectionMatrixLocation = -1

    // 一共 ABCD 四个点，逆时针排序，GLES20.GL_TRIANGLES，ABC、ACD
    var rectVertex = floatArrayOf(
        -0.5f, 0.5f,
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f
    )
    // 一共 OABCD 五个点，逆时针排序，GLES20.GL_TRIANGLE_FAN，OAB、OBC、OCD、ODA
//    var rectVertex = floatArrayOf(
//        0f, 0f,
//        -0.5f, 0.5f,
//        -0.5f, -0.5f,
//        0.5f, -0.5f,
//        0.5f, 0.5f,
//        -0.5f, 0.5f
//    )

    init {
        mProgram = ShaderHelper.buildProgram(context, R.raw.rect_vert_shader, R.raw.rect_frag_shader)

        GLES20.glUseProgram(mProgram)
        vertexArray = VertexArray(rectVertex)
        POSITION_COMPONENT_COUNT = 2
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        aPositionLocation = GLES20.glGetAttribLocation(mProgram, A_POSITION)
        uColorLocation = GLES20.glGetUniformLocation(mProgram, U_COLOR)

        uModelMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_MODEL_MATRIX)
        uViewMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_VIEW_MATRIX)
        uProjectionMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_PROJECTION_MATRIX)

        vertexArray.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        // 因为是 2D 图形，使用正交投影即可
        val aspectRation = if (width > height) width.toFloat() / height else height.toFloat() / width
        if (width > height) {
            Matrix.orthoM(mProjectionMatrix, 0, -aspectRation, aspectRation, -1f, 1f, 0f, 10f)
        } else {
            Matrix.orthoM(mProjectionMatrix, 0, -1f, 1f, -aspectRation, aspectRation, 0f, 10f)
        }
    }

    override fun onDrawFrame(gl: GL10) {
        super.onDrawFrame(gl)
        // 给正交投影矩阵赋值
        GLES20.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, mProjectionMatrix, 0)

        GLES20.glUniform4f(uColorLocation, 0f, 0f, 1f, 1f)
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 6)
    }

}