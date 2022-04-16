package com.app.testopengl.opengl.drawer

import android.content.Context
import android.opengl.GLES20
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.opengl.BaseDrawer
import com.app.testopengl.utils.ShaderHelper
import com.app.testopengl.utils.TextureHelper
import com.app.testopengl.utils.VertexArray
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TextureRectDrawer(context: Context): BaseDrawer(context) {

    private val A_POSITION = "aPosition"
    private val A_TEXTURE_COORDINATES = "aTextureCoordinates"
    private val U_MODEL_MATRIX = "uModelMatrix"
    private val U_VIEW_MATRIX = "uViewMatrix"
    private val U_PROJECTION_MATRIX = "uProjectionMatrix"
    private val U_TEXTURE_UNIT = "uTextureUnit"

    private var aPositionLocation = -1
    private var aTextureCoordinatesLocation = -1
    private var uModelMatrixLocation = -1
    private var uViewMatrixLocation = -1
    private var uProjectionMatrixLocation = -1
    private var uTextureUnitLocation = -1

    private var mTextureId = -1

    // ABD CDB
    // D C
    // A B
    private val vertexArrayData = floatArrayOf(
        -1.0f, -1.0f, // A
        1.0f, -1.0f, // B
        -1.0f, 1.0f, // D
        1.0f, 1.0f // C
    )
    // DCA BAC
    // A B
    // D C
    private val textureArrayData = floatArrayOf(
        0.0f, 1.0f, // D
        1.0f, 1.0f, // C
        0.0f, 0.0f, // A
        1.0f, 0.0f // B
    )

    private var mVertexArray: VertexArray
    private var mTextureArray: VertexArray

    init {
        mProgram = ShaderHelper.buildProgram(mContext, R.raw.texture_vert_shader, R.raw.texture_frag_shader)

        GLES20.glUseProgram(mProgram)
        mVertexArray = VertexArray(vertexArrayData)
        mTextureArray = VertexArray(textureArrayData)
        POSITION_COMPONENT_COUNT = 2
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        GLES20.glClearColor(0f, 0f, 0f, 1.0f)
        aPositionLocation = GLES20.glGetAttribLocation(mProgram, A_POSITION)
        aTextureCoordinatesLocation = GLES20.glGetAttribLocation(mProgram, A_TEXTURE_COORDINATES)
        uModelMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_MODEL_MATRIX)
        uViewMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_VIEW_MATRIX)
        uProjectionMatrixLocation = GLES20.glGetUniformLocation(mProgram, U_PROJECTION_MATRIX)
        uTextureUnitLocation = GLES20.glGetUniformLocation(mProgram, U_TEXTURE_UNIT)

        mTextureId = TextureHelper.loadTexture(mContext, R.drawable.texture)

        GLES20.glUniform1i(uTextureUnitLocation, 0)
        Matrix.setIdentityM(mModelMatrix, 0)
//        Matrix.translateM(mModelMatrix, 0, 0f, 0.5f, 0f)
        Matrix.setIdentityM(mViewMatrix, 0)
        Matrix.setIdentityM(mProjectionMatrix, 0)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        val aspectRation = if (width > height) width.toFloat() / height else height.toFloat() / width
        if (width > height) {
            Matrix.orthoM(mProjectionMatrix, 0, -aspectRation, aspectRation, -1f, 1f, 0f, 10f)
        } else {
            Matrix.orthoM(mProjectionMatrix, 0, -1f, 1f, -aspectRation, aspectRation, 0f, 10f)
        }
    }

    override fun onDrawFrame(gl: GL10) {
        super.onDrawFrame(gl)
        GLES20.glUniformMatrix4fv(uModelMatrixLocation, 1, false, mModelMatrix, 0)
        GLES20.glUniformMatrix4fv(uViewMatrixLocation, 1, false, mViewMatrix, 0)
        GLES20.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, mProjectionMatrix, 0)

        mVertexArray.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0)
        mTextureArray.setVertexAttribPointer(0, aTextureCoordinatesLocation, POSITION_COMPONENT_COUNT, 0)

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTextureId)
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)

        GLES20.glDisableVertexAttribArray(aPositionLocation)
        GLES20.glDisableVertexAttribArray(aTextureCoordinatesLocation)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
    }

}