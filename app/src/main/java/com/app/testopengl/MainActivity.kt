package com.app.testopengl

import com.app.testopengl.base.BaseActivity
import com.app.testopengl.ui.activity.*
import com.blankj.utilcode.constant.PermissionConstants
import com.blankj.utilcode.util.LogUtils
import com.blankj.utilcode.util.PermissionUtils
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : BaseActivity() {

    override fun getLayoutId(): Int = R.layout.activity_main

    override fun initData() {
        btnRequestPermission.setOnClickListener {
            PermissionUtils.permission(PermissionConstants.CAMERA, PermissionConstants.STORAGE, PermissionConstants.MICROPHONE)
                .callback(object : PermissionUtils.SimpleCallback {
                    override fun onGranted() {
                        LogUtils.i("granted")
                    }

                    override fun onDenied() {
                        LogUtils.i("denied")
                    }
                }).request()
        }
        btnDrawBasic2.setOnClickListener {
            BasicShapeActivity.start(mContext)
        }
        btnDrawBasic3.setOnClickListener {
            BasicShape3Activity.start(mContext)
        }
        btnDrawNative.setOnClickListener {
            BasicShapeNativeActivity.start(mContext)
        }
        btnDrawEgl.setOnClickListener {
            EglActivity.start(mContext)
        }
        btnDrawCamera.setOnClickListener {
            CameraActivity.start(mContext)
        }
        btnShowFFmpeg.setOnClickListener {
            FFmpegActivity.start(mContext)
        }
        btnVideoRecorder.setOnClickListener {
            VideoRecorderActivity.start(mContext)
        }
        btnModelRender.setOnClickListener {
            ModelRenderActivity.start(mContext)
        }
        btnBusiness1.setOnClickListener {
            RealViewActivity.start(mContext)
        }
    }
}
