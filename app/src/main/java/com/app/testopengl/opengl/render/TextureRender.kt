package com.app.testopengl.opengl.render

import android.content.Context
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import com.app.testopengl.opengl.drawer.TextureRectDrawer
import com.app.testopengl.opengl.drawer.TextureTriangleDrawer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TextureRender(val context: Context): GLSurfaceView.Renderer {

//    private val mDrawer by lazy { TextureTriangleDrawer(context) }
    private val mDrawer by lazy { TextureRectDrawer(context) }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        mDrawer.onSurfaceCreated(gl, config)
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        mDrawer.onSurfaceChanged(gl, width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        mDrawer.onDrawFrame(gl)
    }
}