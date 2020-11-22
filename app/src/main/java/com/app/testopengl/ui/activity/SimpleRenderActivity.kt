package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.SimpleRender
import com.app.testopengl.opengl.drawer.BitmapDrawer
import com.app.testopengl.opengl.drawer.IDrawer
import com.blankj.utilcode.util.ImageUtils
import kotlinx.android.synthetic.main.activity_simple_render.*

class SimpleRenderActivity : BaseActivity() {

    lateinit var drawer: IDrawer

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, SimpleRenderActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_simple_render

    override fun initData() {
//        drawer = TriangleDrawer()
        drawer = BitmapDrawer(ImageUtils.getBitmap(R.drawable.cover))
        initRender(drawer)
    }

    private fun initRender(drawer: IDrawer) {
        glSurface.setEGLContextClientVersion(2)
        glSurface.setRenderer(SimpleRender(drawer))
    }

    override fun onDestroy() {
        drawer.release()
        super.onDestroy()
    }

}