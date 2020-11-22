package com.app.testopengl.opengl

import android.content.Context
import android.opengl.GLES20
import com.blankj.utilcode.util.Utils
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer

object OpenGLTools {

    fun getFloatBuffer(coors: FloatArray): FloatBuffer {
        val buffer: FloatBuffer
        // 每个浮点数:坐标个数* 4字节
        val qbb: ByteBuffer = ByteBuffer.allocateDirect(coors.size * 4)
        // 使用本机硬件设备的字节顺序
        qbb.order(ByteOrder.nativeOrder())
        // 从字节缓冲区创建浮点缓冲区
        buffer = qbb.asFloatBuffer()
        // 将坐标添加到FloatBuffer
        buffer.put(coors)
        // 设置缓冲区以读取第一个坐标
        buffer.position(0)
        return buffer
    }

    fun initProgramByAssets(vertexName: String, fragName: String): Int {
        // 顶点着色
        val vertexShader = loadShaderAssets(Utils.getApp(), GLES20.GL_VERTEX_SHADER, vertexName)
        // 片元着色
        val fragmentShader = loadShaderAssets(Utils.getApp(), GLES20.GL_FRAGMENT_SHADER, fragName)
        val program = GLES20.glCreateProgram() // 创建空的OpenGL ES 程序
        GLES20.glAttachShader(program, vertexShader) // 加入顶点着色器
        GLES20.glAttachShader(program, fragmentShader) // 加入片元着色器
        GLES20.glLinkProgram(program) // 创建可执行的OpenGL ES项目
        return program
    }

    private fun loadShaderAssets(ctx: Context, type: Int, name: String): Int {
        val buf = ByteArray(1024)
        val sb = StringBuilder()
        var len: Int
        try {
            ctx.assets.open(name).use { stream ->
                while (stream.read(buf).also { len = it } != -1) {
                    sb.append(String(buf, 0, len))
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return loadShader(type, sb.toString())
    }

    private fun loadShader(type: Int, shaderCode: String): Int {
        // 根据type创建顶点着色器或者片元着色器
        val shader = GLES20.glCreateShader(type)
        // 将资源加入到着色器中，并编译
        GLES20.glShaderSource(shader, shaderCode)
        GLES20.glCompileShader(shader)
        return shader
    }

    fun createTextureIds(count: Int): IntArray {
        val texture = IntArray(count)
        GLES20.glGenTextures(count, texture, 0) //生成纹理
        return texture
    }

}