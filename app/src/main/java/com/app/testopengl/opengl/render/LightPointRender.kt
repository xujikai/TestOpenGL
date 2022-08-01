package com.app.testopengl.opengl.render

import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.utils.GLComUtils
import com.app.testopengl.utils.GLTexUtils
import java.nio.FloatBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class LightPointRender: GLSurfaceView.Renderer {

    private var mProgram = -1

    private val aPositionLocation = 0
    private val aTextureCoordinatesLocation = 1
    private val aNormalLocation = 2
    private var uModelMatrixLocation = -1
    private var uViewMatrixLocation = -1
    private var uProjectionMatrixLocation = -1
    private var uImageTextureLocation = -1
    private var uLightPositionLocation = -1
    private var uViewPositionLocation = -1
    private var uLightColorLocation = -1
    private var uObjectColorLocation = -1

    private val mVertexArr = floatArrayOf(
        // 正面
        // front face
        -1f, -1f, 1f,
        -1f, 1f, 1f,
        1f, 1f, 1f,
        -1f, -1f, 1f,
        1f, 1f, 1f,
        1f, -1f, 1f,
        // 背面
        // back face
        -1f, -1f, -1f,
        -1f, 1f, -1f,
        1f, 1f, -1f,
        -1f, -1f, -1f,
        1f, 1f, -1f,
        1f, -1f, -1f,
        // 顶面
        // Top face
        -1f, 1f, -1f,
        -1f, 1f, 1f,
        1f, 1f, 1f,
        -1f, 1f, -1f,
        1f, 1f, 1f,
        1f, 1f, -1f,
        // 底面
        // Bottom face
        -1f, -1f, -1f,
        -1f, -1f, 1f,
        1f, -1f, 1f,
        -1f, -1f, -1f,
        1f, -1f, 1f,
        1f, -1f, -1f,
        // 左面
        // Left face
        -1f, -1f, -1f,
        -1f, -1f, 1f,
        -1f, 1f, 1f,
        -1f, -1f, -1f,
        -1f, 1f, 1f,
        -1f, 1f, -1f,
        // 右面
        // Right face
        1f, -1f, -1f,
        1f, -1f, 1f,
        1f, 1f, 1f,
        1f, -1f, -1f,
        1f, 1f, 1f,
        1f, 1f, -1f
    )
    private val mNormalArr = floatArrayOf(
        // 正面
        // front face
        0f, 0f, 1f,
        0f, 0f, 1f,
        0f, 0f, 1f,
        0f, 0f, 1f,
        0f, 0f, 1f,
        0f, 0f, 1f,
        // 背面
        // back face
        0f, 0f, -1f,
        0f, 0f, -1f,
        0f, 0f, -1f,
        0f, 0f, -1f,
        0f, 0f, -1f,
        0f, 0f, -1f,
        // 顶面
        // Top face
        0f, 1f, 0f,
        0f, 1f, 0f,
        0f, 1f, 0f,
        0f, 1f, 0f,
        0f, 1f, 0f,
        0f, 1f, 0f,
        // 底面
        // Bottom face
        0f, -1f, 0f,
        0f, -1f, 0f,
        0f, -1f, 0f,
        0f, -1f, 0f,
        0f, -1f, 0f,
        0f, -1f, 0f,
        // 左面
        // Left face
        -1f, 0f, 0f,
        -1f, 0f, 0f,
        -1f, 0f, 0f,
        -1f, 0f, 0f,
        -1f, 0f, 0f,
        -1f, 0f, 0f,
        // 右面
        // Right face
        1f, 0f, 0f,
        1f, 0f, 0f,
        1f, 0f, 0f,
        1f, 0f, 0f,
        1f, 0f, 0f,
        1f, 0f, 0f
    )
    private val mTextureArr = floatArrayOf(
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f,
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f,
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f,
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f,
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f,
        0f, 1f,
        0f, 0f,
        1f, 0f,
        0f, 1f,
        1f, 0f,
        1f, 1f
    )
    private lateinit var mVertexBuffer: FloatBuffer
    private lateinit var mNormalBuffer: FloatBuffer
    private lateinit var mTextureBuffer: FloatBuffer

    private var mImageTexture = -1

    var rotateY = 0f
    var cameraPositionX = 0f
    var cameraPositionY = 0f
    var cameraPositionZ = 5f
    var lookAtX = 0f
    var lookAtY = 0f
    var lookAtZ = 0f
    var cameraUpX = 0f
    var cameraUpY = 1f
    var cameraUpZ = 0f
    var nearPlaneLeft = -1f
    var nearPlaneRight = 1f
    var nearPlaneBottom = -1f
    var nearPlaneTop = 1f
    var nearPlane = 2f
    var farPlane = 100f

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram = GLComUtils.createProgram(R.raw.light_point_vert_shader, R.raw.light_point_frag_shader)

        uModelMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uModelMatrix")
        uViewMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uViewMatrix")
        uProjectionMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uProjectionMatrix")
        uImageTextureLocation = GLES30.glGetUniformLocation(mProgram, "uImageTexture")
        uLightPositionLocation = GLES30.glGetUniformLocation(mProgram, "uLightPosition")
        uViewPositionLocation = GLES30.glGetUniformLocation(mProgram, "uViewPosition")
        uLightColorLocation = GLES30.glGetUniformLocation(mProgram, "uLightColor")
        uObjectColorLocation = GLES30.glGetUniformLocation(mProgram, "uObjectColor")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)
        mNormalBuffer = GLComUtils.createFloatBuffer(mNormalArr)
        mTextureBuffer = GLComUtils.createFloatBuffer(mTextureArr)

        mImageTexture = GLTexUtils.loadTexture(R.drawable.brick)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
        nearPlaneBottom = -height.toFloat() / width
        nearPlaneTop = height.toFloat() / width
    }

    override fun onDrawFrame(gl: GL10) {
        rotateY += 0.2f

        GLES30.glEnable(GLES30.GL_DEPTH_TEST)

        GLES30.glClearColor(0f, 0f, 0f, 0f)
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT or GLES30.GL_DEPTH_BUFFER_BIT)

        GLES30.glUseProgram(mProgram)

        GLES30.glEnableVertexAttribArray(aPositionLocation)
        GLES30.glVertexAttribPointer(aPositionLocation, 3, GLES30.GL_FLOAT, false, 0, mVertexBuffer)
        GLES30.glEnableVertexAttribArray(aTextureCoordinatesLocation)
        GLES30.glVertexAttribPointer(aTextureCoordinatesLocation, 2, GLES30.GL_FLOAT, false, 0, mTextureBuffer)
        GLES30.glEnableVertexAttribArray(aNormalLocation)
        GLES30.glVertexAttribPointer(aNormalLocation, 3, GLES30.GL_FLOAT, false, 0, mNormalBuffer)

        val modelMatrix = GLComUtils.getIdentity()
        val viewMatrix = GLComUtils.getIdentity()
        val projectionMatrix = GLComUtils.getIdentity()

        Matrix.rotateM(modelMatrix, 0, rotateY, 0f, 1f, 0f)
        Matrix.setLookAtM(viewMatrix, 0, cameraPositionX, cameraPositionY, cameraPositionZ,
            lookAtX, lookAtY, lookAtZ, cameraUpX, cameraUpY, cameraUpZ)
        Matrix.frustumM(projectionMatrix, 0, nearPlaneLeft, nearPlaneRight, nearPlaneBottom, nearPlaneTop, nearPlane, farPlane)

        GLES30.glUniformMatrix4fv(uModelMatrixLocation, 1, false, modelMatrix, 0)
        GLES30.glUniformMatrix4fv(uViewMatrixLocation, 1, false, viewMatrix, 0)
        GLES30.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, projectionMatrix, 0)

        GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mImageTexture)
        GLES30.glUniform1i(uImageTextureLocation, 0)
        GLES30.glUniform3f(uLightPositionLocation, 5f, 0f, 0f)
        GLES30.glUniform3f(uViewPositionLocation, 0f, 0f, 5f)
        GLES30.glUniform3f(uLightColorLocation, 1f, 1f, 1f)
        GLES30.glUniform3f(uObjectColorLocation, 1f, 1f, 0f)

        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, mVertexArr.size / 3)
    }

}