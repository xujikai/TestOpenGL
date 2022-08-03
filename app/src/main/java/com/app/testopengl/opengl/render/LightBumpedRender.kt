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
import kotlin.math.sqrt

class LightBumpedRender : GLSurfaceView.Renderer {

    private var mProgram = -1

    private val aPositionLocation = 0
    private val aTextureCoordinatesLocation = 1
    private val aNormalLocation = 2
    private val aTangentLocation = 3
    private val aBitangentLocation = 4
    private var uModelMatrixLocation = -1
    private var uViewMatrixLocation = -1
    private var uProjectionMatrixLocation = -1
    private var uImageTextureLocation = -1
    private var uNormalTextureLocation = -1
    private var uLightPositionLocation = -1
    private var uViewPositionLocation = -1
    private var uLightColorLocation = -1

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
    private lateinit var mTangentBuffer: FloatBuffer
    private lateinit var mBitangentBuffer: FloatBuffer

    private var mImageTexture = -1
    private var mNormalTexture = -1

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
        mProgram = GLComUtils.createProgram(R.raw.light_bumped_vert_shader, R.raw.light_bumped_frag_shader)

        uModelMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uModelMatrix")
        uViewMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uViewMatrix")
        uProjectionMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uProjectionMatrix")
        uImageTextureLocation = GLES30.glGetUniformLocation(mProgram, "uImageTexture")
        uNormalTextureLocation = GLES30.glGetUniformLocation(mProgram, "uNormalTexture")
        uLightPositionLocation = GLES30.glGetUniformLocation(mProgram, "uLightPosition")
        uViewPositionLocation = GLES30.glGetUniformLocation(mProgram, "uViewPosition")
        uLightColorLocation = GLES30.glGetUniformLocation(mProgram, "uLightColor")

        mVertexBuffer = GLComUtils.createFloatBuffer(mVertexArr)
        mNormalBuffer = GLComUtils.createFloatBuffer(mNormalArr)
        mTextureBuffer = GLComUtils.createFloatBuffer(mTextureArr)

        mImageTexture = GLTexUtils.loadTexture(R.drawable.brick)
        mNormalTexture = GLTexUtils.loadTexture(R.drawable.brickn)

        val tangentList = mutableListOf<Float>()
        val bitangentList = mutableListOf<Float>()
        for (i in 0 until mVertexArr.size / 9) {
            val e1x = mVertexArr[i * 9 + 3] - mVertexArr[i * 9]
            val e1y = mVertexArr[i * 9 + 3 + 1] - mVertexArr[i * 9 + 1]
            val e1z = mVertexArr[i * 9 + 3 + 2] - mVertexArr[i * 9 + 2]
            val e2x = mVertexArr[i * 9 + 6] - mVertexArr[i * 9]
            val e2y = mVertexArr[i * 9 + 6 + 1] - mVertexArr[i * 9 + 1]
            val e2z = mVertexArr[i * 9 + 6 + 2] - mVertexArr[i * 9 + 2]
            val deltaUV1x = mTextureArr[i * 6 + 2] - mTextureArr[i * 6]
            val deltaUV1y = mTextureArr[i * 6 + 2 + 1] - mTextureArr[i * 6 + 1]
            val deltaUV2x = mTextureArr[i * 6 + 4] - mTextureArr[i * 6]
            val deltaUV2y = mTextureArr[i * 6 + 4 + 1] - mTextureArr[i * 6 + 1]
            val f = 1f / (deltaUV1x * deltaUV2y - deltaUV2x * deltaUV1y)
            var tangentx = f * (deltaUV2y * e1x - deltaUV1y * e2x)
            var tangenty = f * (deltaUV2y * e1y - deltaUV1y * e2y)
            var tangentz = f * (deltaUV2y * e1z - deltaUV1y * e2z)
            val tangentLength = sqrt(tangentx * tangentx + tangenty * tangenty + tangentz * tangentz)
            tangentx /= tangentLength
            tangenty /= tangentLength
            tangentz /= tangentLength
            var bitangentx = f * (-deltaUV2x * e1x + deltaUV1x * e2x)
            var bitangenty = f * (-deltaUV2x * e1y + deltaUV1x * e2y)
            var bitangentz = f * (-deltaUV2x * e1z + deltaUV1x * e2z)
            val bitangentLength = sqrt(bitangentx * bitangentx + bitangenty * bitangenty + bitangentz * bitangentz)
            bitangentx /= bitangentLength
            bitangenty /= bitangentLength
            bitangentz /= bitangentLength
            for (j in 0 until 9) {
                when (j % 3) {
                    0 -> {
                        tangentList.add(tangentx)
                        bitangentList.add(bitangentx)
                    }
                    1 -> {
                        tangentList.add(tangenty)
                        bitangentList.add(bitangenty)
                    }
                    2 -> {
                        tangentList.add(tangentz)
                        bitangentList.add(bitangentz)
                    }
                }
            }
        }
        mTangentBuffer = GLComUtils.createFloatBuffer(tangentList.toFloatArray())
        mBitangentBuffer = GLComUtils.createFloatBuffer(bitangentList.toFloatArray())
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
        GLES30.glEnableVertexAttribArray(aTangentLocation)
        GLES30.glVertexAttribPointer(aTangentLocation, 3, GLES30.GL_FLOAT, false, 0, mTangentBuffer)
        GLES30.glEnableVertexAttribArray(aBitangentLocation)
        GLES30.glVertexAttribPointer(aBitangentLocation, 3, GLES30.GL_FLOAT, false, 0, mBitangentBuffer)

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
        GLES30.glActiveTexture(GLES30.GL_TEXTURE1)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mNormalTexture)
        GLES30.glUniform1i(uNormalTextureLocation, 1)

        GLES30.glUniform3f(uLightPositionLocation, 2f, 0f, 2f)
        GLES30.glUniform3f(uViewPositionLocation, 0f, 0f, 5f)
        GLES30.glUniform3f(uLightColorLocation, 1f, 1f, 1f)

        GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, mVertexArr.size / 3)
    }

}