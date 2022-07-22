package com.app.testopengl.opengl.render

import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.app.testopengl.R
import com.app.testopengl.utils.Constants
import com.app.testopengl.utils.GLComUtils
import com.app.testopengl.utils.GLTexUtils
import java.nio.FloatBuffer
import java.nio.IntBuffer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class VBORender: GLSurfaceView.Renderer {

    private var mProgram = -1

    private var aPositionLocation = 0
    private var aTextureCoordinatesLocation = 1
    private var uProjectionMatrixLocation = -1
    private var uTextureLocation = -1

    // 顶点 + 纹理
    private val mVertexTextureArr = floatArrayOf(
        -1f, -1f,   0f, 1f,     // x, y, u, v
        -1f, 1f,    0f, 0f,
        1f, 1f,     1f, 0f,
        1f, -1f,    1f, 1f
    )
    private val mIndexArr = intArrayOf(0, 1, 2, 0, 2, 3)
    private lateinit var mVertexTextureBuffer: FloatBuffer
    private lateinit var mIndexBuffer: IntBuffer

    private var mProjectionMatrix = FloatArray(16)

    private var mVboId = 0
    private var mEboId = 0

    private var mTextureId = -1

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mProgram = GLComUtils.createProgram(R.raw.vbo_vert_shader, R.raw.vbo_frag_shader)

        uProjectionMatrixLocation = GLES30.glGetUniformLocation(mProgram, "uProjectionMatrix")
        uTextureLocation = GLES30.glGetUniformLocation(mProgram, "uTexture")

        mVertexTextureBuffer = GLComUtils.createFloatBuffer(mVertexTextureArr)
        mIndexBuffer = GLComUtils.createIntBuffer(mIndexArr)

        val buffers = IntArray(2)
        GLES30.glGenBuffers(buffers.size, buffers, 0)
        mVboId = buffers[0]
        mEboId = buffers[1]
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVboId)
        /*
         * @param target 目标缓冲的类型
         * @param size 指定传输数据的大小(以字节为单位)
         * @param usage 指定显卡如何管理给定的数据，它有三种形式
         *      GL_STATIC_DRAW ：数据不会或几乎不会改变
         *      GL_DYNAMIC_DRAW：数据会被改变很多
         *      GL_STREAM_DRAW ：数据每次绘制时都会改变
         */
        GLES30.glBufferData(GLES30.GL_ARRAY_BUFFER, mVertexTextureBuffer.capacity() * Constants.BYTES_PRE_FLOAT, mVertexTextureBuffer, GLES30.GL_STATIC_DRAW)
        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, GLES30.GL_NONE)
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, mEboId)
        GLES30.glBufferData(GLES30.GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer.capacity() * Constants.BYTES_PRE_FLOAT, mIndexBuffer, GLES30.GL_STATIC_DRAW)
        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, GLES30.GL_NONE)

        mTextureId = GLTexUtils.loadTexture(R.drawable.texture)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
        if (width > height) {
            val aspect = width.toFloat() / height
            Matrix.orthoM(mProjectionMatrix, 0, -aspect, aspect, -1f, 1f, 0f, 10f)
        } else {
            val aspect = height.toFloat() / width
            Matrix.orthoM(mProjectionMatrix, 0, -1f, 1f, -aspect, aspect, 0f, 10f)
        }
    }

    override fun onDrawFrame(gl: GL10) {
        GLES30.glClearColor(0f, 0f, 0f, 1f)
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        GLES30.glUseProgram(mProgram)

        GLES30.glBindBuffer(GLES30.GL_ARRAY_BUFFER, mVboId)
        GLES30.glEnableVertexAttribArray(aPositionLocation)
        GLES30.glVertexAttribPointer(aPositionLocation, 2, GLES30.GL_FLOAT, false, 4 * Constants.BYTES_PRE_FLOAT, 0)
        GLES30.glEnableVertexAttribArray(aTextureCoordinatesLocation)
        GLES30.glVertexAttribPointer(aTextureCoordinatesLocation, 2, GLES30.GL_FLOAT, false, 4 * Constants.BYTES_PRE_FLOAT, 2 * Constants.BYTES_PRE_FLOAT)

        GLES30.glEnableVertexAttribArray(uProjectionMatrixLocation)
        GLES30.glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, mProjectionMatrix, 0)

        GLES30.glActiveTexture(GLES30.GL_TEXTURE0)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, mTextureId)
        GLES30.glUniform1i(uTextureLocation, 0)

        GLES30.glBindBuffer(GLES30.GL_ELEMENT_ARRAY_BUFFER, mEboId)
        GLES30.glDrawElements(GLES30.GL_TRIANGLES, mIndexArr.size, GLES30.GL_UNSIGNED_INT, 0)

        GLES30.glDisableVertexAttribArray(aPositionLocation)
        GLES30.glDisableVertexAttribArray(aTextureCoordinatesLocation)
        GLES30.glDisableVertexAttribArray(uProjectionMatrixLocation)
        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, GLES30.GL_NONE)
        GLES30.glUseProgram(GLES30.GL_NONE)
    }
}