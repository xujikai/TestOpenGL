package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
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
import com.app.testopengl.ffmpeg.AudioRecorder
import com.app.testopengl.ffmpeg.FFMediaRecorder
import com.app.testopengl.opengl.RenderCons
import com.blankj.utilcode.util.LogUtils
import com.blankj.utilcode.util.ScreenUtils
import com.blankj.utilcode.util.ThreadUtils
import kotlinx.android.synthetic.main.activity_video_recorder.*

class VideoRecorderActivity: BaseActivity(), Camera2FrameCallback,
    AudioRecorder.AudioRecorderCallback {

    lateinit var mSurfaceViewRoot: RelativeLayout
    lateinit var mGLSurfaceView: GLSurfaceView

    lateinit var mCamera2Wrapper: Camera2Wrapper
    lateinit var mMediaRecorder: FFMediaRecorder
    lateinit var mAudioRecorder: AudioRecorder
    var mRecorderType = RenderCons.RECORDER_TYPE_AV

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, VideoRecorderActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_video_recorder

    override fun initData() {
        mSurfaceViewRoot = findViewById(R.id.surface_root)
        mGLSurfaceView = GLSurfaceView(this)

        mCamera2Wrapper = Camera2Wrapper(this)
        mMediaRecorder = FFMediaRecorder()

        val p = RelativeLayout.LayoutParams(
            RelativeLayout.LayoutParams.MATCH_PARENT,
            RelativeLayout.LayoutParams.MATCH_PARENT
        )
        mSurfaceViewRoot.addView(mGLSurfaceView, p)
        mMediaRecorder.init(mGLSurfaceView)

        val treeObserver = mSurfaceViewRoot.viewTreeObserver
        treeObserver.addOnPreDrawListener(object : ViewTreeObserver.OnPreDrawListener {
            override fun onPreDraw(): Boolean {
                mSurfaceViewRoot.viewTreeObserver.removeOnPreDrawListener(this)
                updateGLSurfaceViewSize(mCamera2Wrapper.previewSize)
                return true
            }
        })

        btnStartRecord.setOnClickListener {
            val frameWidth = mCamera2Wrapper.previewSize.height // 720
            val frameHeight = mCamera2Wrapper.previewSize.width // 1280
            val fps = 25
            val bitRate = (frameWidth * frameHeight * fps * 0.25).toLong()
            mMediaRecorder.startRecord(mRecorderType, getVideoPath(), frameWidth, frameHeight, bitRate, fps)
            if (mRecorderType == RenderCons.RECORDER_TYPE_AV) {
                mAudioRecorder = AudioRecorder(this)
                mAudioRecorder.start()
            }
        }
        btnStopRecord.setOnClickListener {
            if (mRecorderType == RenderCons.RECORDER_TYPE_AV) {
                mAudioRecorder.interrupt()
            }
            ThreadUtils.getCachedPool().execute {
                mMediaRecorder.stopRecord()
                if (mRecorderType == RenderCons.RECORDER_TYPE_AV) {
                    runOnUiThread {
                        mAudioRecorder.join()
                    }
                }
            }
        }
    }

    override fun onResume() {
        super.onResume()
        mCamera2Wrapper.startCamera()
        updateTransformMatrix(mCamera2Wrapper.cameraId)
        updateGLSurfaceViewSize(mCamera2Wrapper.previewSize)
    }

    override fun onPause() {
        super.onPause()
        mCamera2Wrapper.stopCamera()
    }

    override fun onDestroy() {
        super.onDestroy()
        mMediaRecorder.unInit()
    }

    override fun onPreviewFrame(data: ByteArray, width: Int, height: Int) {
//        LogUtils.i("$width $height")
        mMediaRecorder.onPreviewFrame(RenderCons.IMAGE_FORMAT_I420, width, height, data)
        mMediaRecorder.requestRender()
    }

    override fun onCaptureFrame(data: ByteArray, width: Int, height: Int) {

    }

    override fun onAudioData(data: ByteArray, dataSize: Int) {
        mMediaRecorder.onAudioData(data)
    }

    override fun onError(msg: String) {
        LogUtils.e(msg)
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
            mMediaRecorder.setTransformMatrix(90, 0)
        } else {
            mMediaRecorder.setTransformMatrix(90, 1)
        }
    }

    private fun getVideoPath(): String {
        return "${externalCacheDir?.path}/mediacodec_demo.mp4"
    }

}