package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.hardware.camera2.CameraCharacteristics
import android.opengl.GLSurfaceView
import android.util.Size
import android.view.ViewTreeObserver
import android.widget.RelativeLayout
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.camera.Camera2FrameCallback
import com.app.testopengl.camera.Camera2Wrapper
import com.app.testopengl.camera.CameraUtil
import com.app.testopengl.opengl.RealViewRender
import com.app.testopengl.opengl.RenderCons
import com.app.testopengl.opengl.RenderCons.IMAGE_FORMAT_RGBA
import com.blankj.utilcode.util.LogUtils
import com.blankj.utilcode.util.ScreenUtils
import java.nio.ByteBuffer

class RealViewActivity : BaseActivity(), Camera2FrameCallback {

    lateinit var mSurfaceViewRoot: RelativeLayout
    lateinit var mGLSurfaceView: GLSurfaceView

    lateinit var mCameraRender: RealViewRender
    lateinit var mCamera2Wrapper: Camera2Wrapper

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, RealViewActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_real_view

    override fun initData() {
        mSurfaceViewRoot = findViewById(R.id.surface_root)
        mGLSurfaceView = GLSurfaceView(this)

        mCameraRender = RealViewRender()
        mCamera2Wrapper = Camera2Wrapper(this)

        val p = RelativeLayout.LayoutParams(
            RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.MATCH_PARENT
        )
        mSurfaceViewRoot.addView(mGLSurfaceView, p)

        mGLSurfaceView.setEGLContextClientVersion(3)
        mGLSurfaceView.setRenderer(mCameraRender)
        mGLSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        mCameraRender.init()
        loadRGBAImage(R.drawable.avatar_b)

        val treeObserver = mSurfaceViewRoot.viewTreeObserver
        treeObserver.addOnPreDrawListener(object : ViewTreeObserver.OnPreDrawListener {
            override fun onPreDraw(): Boolean {
                mSurfaceViewRoot.viewTreeObserver.removeOnPreDrawListener(this)
                updateGLSurfaceViewSize(mCamera2Wrapper.previewSize)
                return true
            }
        })
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
        mCameraRender.setImageData(RenderCons.IMAGE_FORMAT_I420, width, height, data)
        mGLSurfaceView.requestRender()
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

    private fun loadRGBAImage(resId: Int) {
        val inputStream = resources.openRawResource(resId)
        val bitmap: Bitmap = BitmapFactory.decodeStream(inputStream)
        val buf = ByteBuffer.allocate(bitmap.byteCount)
        bitmap.copyPixelsToBuffer(buf)
        mCameraRender.setObjImageData(IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
        inputStream.close()
    }

}