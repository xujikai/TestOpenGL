package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.render.TextureRender
import kotlinx.android.synthetic.main.activity_texture.*

class TextureActivity: BaseActivity() {

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, TextureActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_texture

    override fun initData() {
        glSurface.setEGLContextClientVersion(2)
        glSurface.setRenderer(TextureRender(this))
    }

}