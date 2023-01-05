package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.hardware.camera2.CameraCharacteristics
import android.opengl.GLSurfaceView
import android.util.Size
import android.view.ViewTreeObserver.OnPreDrawListener
import android.widget.RelativeLayout
import androidx.appcompat.widget.AppCompatButton
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.camera.Camera2FrameCallback
import com.app.testopengl.camera.Camera2Wrapper
import com.app.testopengl.camera.CameraUtil
import com.app.testopengl.opengl.MyCameraRender
import com.app.testopengl.opengl.RenderCons
import com.blankj.utilcode.util.LogUtils
import com.blankj.utilcode.util.ScreenUtils

class CameraActivity: BaseActivity(), Camera2FrameCallback {

    lateinit var mSurfaceViewRoot: RelativeLayout
    lateinit var mGLSurfaceView: GLSurfaceView

    lateinit var mCameraRender: MyCameraRender
    lateinit var mCamera2Wrapper: Camera2Wrapper

    private var mCurShaderIndex = 0
    private val MIN_SHADER_INDEX = 0
    private val MAX_SHADER_INDEX = 4

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, CameraActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_camera

    override fun initData() {
        mSurfaceViewRoot = findViewById(R.id.surface_root)
        mGLSurfaceView = GLSurfaceView(this)

        mCameraRender = MyCameraRender()
        mCamera2Wrapper = Camera2Wrapper(this)

        val p = RelativeLayout.LayoutParams(
            RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.MATCH_PARENT
        )
        mSurfaceViewRoot.addView(mGLSurfaceView, p)
        mCameraRender.init(mGLSurfaceView)

        val treeObserver = mSurfaceViewRoot.viewTreeObserver
        treeObserver.addOnPreDrawListener(object : OnPreDrawListener {
            override fun onPreDraw(): Boolean {
                mSurfaceViewRoot.viewTreeObserver.removeOnPreDrawListener(this)
                updateGLSurfaceViewSize(mCamera2Wrapper.previewSize)
                return true
            }
        })

        findViewById<AppCompatButton>(R.id.btnPrev).setOnClickListener {
            mCurShaderIndex--
            if (mCurShaderIndex < MIN_SHADER_INDEX) {
                mCurShaderIndex = MAX_SHADER_INDEX
            }
            mCameraRender.loadShaderFromAssets(mCurShaderIndex)
        }
        findViewById<AppCompatButton>(R.id.btnNext).setOnClickListener {
            mCurShaderIndex++
            if (mCurShaderIndex > MAX_SHADER_INDEX) {
                mCurShaderIndex = MIN_SHADER_INDEX
            }
            mCameraRender.loadShaderFromAssets(mCurShaderIndex)
        }
    }

    override fun onResume() {
        super.onResume()
        mCamera2Wrapper.startCamera()
        updateTransformMatrix(mCamera2Wrapper.cameraId)
        updateGLSurfaceViewSize(mCamera2Wrapper.previewSize)
    }

    override fun onPause() {
        mCamera2Wrapper.stopCamera()
        super.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        mCameraRender.unInit()
    }

    override fun onPreviewFrame(data: ByteArray, width: Int, height: Int) {
//        LogUtils.i("$width $height")
        mCameraRender.setRenderFrame(RenderCons.IMAGE_FORMAT_I420, width, height, data)
        mCameraRender.requestRender()
    }

    override fun onCaptureFrame(data: ByteArray, width: Int, height: Int) {

    }

    fun updateGLSurfaceViewSize(previewSize: Size) {
        LogUtils.i(previewSize)
        val fitSize = CameraUtil.getFitInScreenSize(
            previewSize.width,
            previewSize.height,
            ScreenUtils.getAppScreenWidth(),
            ScreenUtils.getAppScreenHeight()
        )
        val params = mGLSurfaceView.layoutParams as RelativeLayout.LayoutParams
        params.width = fitSize.width
        params.height = fitSize.height
        params.addRule(RelativeLayout.ALIGN_PARENT_TOP or RelativeLayout.CENTER_HORIZONTAL)
        mGLSurfaceView.layoutParams = params
    }

    fun updateTransformMatrix(cameraId: String) {
        if (cameraId == CameraCharacteristics.LENS_FACING_FRONT.toString()) {
            mCameraRender.setTransformMatrix(0f, 0f, 1f, 1f, 90, 0)
        } else {
            mCameraRender.setTransformMatrix(0f, 0f, 1f, 1f, 90, 1)
        }
    }

}