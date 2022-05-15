package com.app.testopengl

import com.app.testopengl.base.BaseActivity
import com.app.testopengl.ui.activity.*
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : BaseActivity() {

    override fun getLayoutId(): Int = R.layout.activity_main

    override fun initData() {
        btnDrawPoint.setOnClickListener {
            BasicShapeActivity.start(mContext)
        }
        btnDrawTexture.setOnClickListener {
            TextureActivity.start(mContext)
        }
        btnDrawTriangle.setOnClickListener {
            TriangleActivity.start(mContext)
        }
        btnDrawEgl.setOnClickListener {
            EglActivity.start(mContext)
        }
    }
}
