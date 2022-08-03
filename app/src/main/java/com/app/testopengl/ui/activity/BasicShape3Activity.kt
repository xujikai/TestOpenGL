package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.render.*
import kotlinx.android.synthetic.main.activity_basic_shape.*

/**
 * OpenGL 3.0
 */
class BasicShape3Activity: BaseActivity() {

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, BasicShape3Activity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_basic_shape

    override fun initData() {
        glSurface.setEGLConfigChooser(8, 8, 8, 8, 8, 0)
        glSurface.setEGLContextClientVersion(3)
//        glSurface.setRenderer(VBORender())
//        glSurface.setRenderer(MRTRender())
//        glSurface.setRenderer(LightDirectionRender())
//        glSurface.setRenderer(LightPointRender())
//        glSurface.setRenderer(LightSpotRender())
        glSurface.setRenderer(LightBumpedRender())
    }

}