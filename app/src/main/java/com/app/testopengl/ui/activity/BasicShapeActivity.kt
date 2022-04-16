package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.render.PointRender
import kotlinx.android.synthetic.main.activity_basic_shape.glSurface

/**
 * 基本形状的绘制
 */
class BasicShapeActivity: BaseActivity() {

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, BasicShapeActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_basic_shape

    override fun initData() {
        glSurface.setEGLContextClientVersion(2)
        glSurface.setRenderer(PointRender(this))
//        glSurface.setRenderer(LineRender(this))
//        glSurface.setRenderer(TriangleRender(this))
//        glSurface.setRenderer(RectRender(this))
//        glSurface.setRenderer(CircleRender(this))
    }

}