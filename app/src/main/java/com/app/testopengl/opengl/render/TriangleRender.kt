package com.app.testopengl.opengl.render

import android.content.Context
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.opengl.drawer.TriangleDrawer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TriangleRender(val context: Context): GLSurfaceView.Renderer {

    private lateinit var mTriangleDrawer: TriangleDrawer

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mTriangleDrawer = TriangleDrawer(context)
        mTriangleDrawer.onSurfaceCreated(gl, config)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        mTriangleDrawer.onSurfaceChanged(gl, width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mTriangleDrawer.onDrawFrame(gl)
    }
}