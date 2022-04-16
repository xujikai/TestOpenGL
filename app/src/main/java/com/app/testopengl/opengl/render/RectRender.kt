package com.app.testopengl.opengl.render

import android.content.Context
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.opengl.drawer.PointDrawer
import com.app.testopengl.opengl.drawer.RectDrawer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class RectRender(val context: Context): GLSurfaceView.Renderer {

    private lateinit var mRectDrawer: RectDrawer

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mRectDrawer = RectDrawer(context)
        mRectDrawer.onSurfaceCreated(gl, config)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        mRectDrawer.onSurfaceChanged(gl, width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mRectDrawer.onDrawFrame(gl)
    }
}