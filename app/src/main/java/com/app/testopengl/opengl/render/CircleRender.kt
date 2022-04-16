package com.app.testopengl.opengl.render

import android.content.Context
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.opengl.drawer.CircleDrawer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class CircleRender(val context: Context): GLSurfaceView.Renderer {

    private lateinit var mCircleDrawer: CircleDrawer

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mCircleDrawer = CircleDrawer(context)
        mCircleDrawer.onSurfaceCreated(gl, config)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        mCircleDrawer.onSurfaceChanged(gl, width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mCircleDrawer.onDrawFrame(gl)
    }
}