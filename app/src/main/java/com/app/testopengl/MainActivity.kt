package com.app.testopengl

import com.app.testopengl.base.BaseActivity
import com.app.testopengl.ui.activity.OpenGLPlayerActivity
import com.app.testopengl.ui.activity.SimpleRenderActivity
import com.blankj.utilcode.constant.PermissionConstants
import com.blankj.utilcode.util.PermissionUtils
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : BaseActivity() {

    override fun getLayoutId(): Int = R.layout.activity_main

    override fun initData() {
        PermissionUtils.permission(PermissionConstants.STORAGE)
            .request()

        // Example of a call to a native method
        sample_text.text = "${stringFromJNI()} ${stringFromJNI2()}"

        btnDrawTriangle.setOnClickListener {
            SimpleRenderActivity.start(mContext)
        }
        btnOpenGLPlayer.setOnClickListener {
            OpenGLPlayerActivity.start(mContext)
        }
    }

    external fun stringFromJNI(): String

    external fun stringFromJNI2(): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
