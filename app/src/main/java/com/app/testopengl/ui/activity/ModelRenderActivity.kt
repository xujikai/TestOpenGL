package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.util.Log
import android.view.ViewTreeObserver
import androidx.lifecycle.lifecycleScope
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.bean.MotionBean
import com.app.testopengl.ffmpeg.AudioRecorder
import com.app.testopengl.ffmpeg.FFMediaRecorder
import com.app.testopengl.opengl.RenderCons
import com.blankj.utilcode.util.GsonUtils
import com.blankj.utilcode.util.LogUtils
import com.blankj.utilcode.util.ResourceUtils
import com.blankj.utilcode.util.ThreadUtils
import com.google.android.filament.Fence
import com.google.android.filament.utils.*
import kotlinx.android.synthetic.main.activity_model_render.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import java.nio.ByteBuffer
import kotlin.system.measureTimeMillis


class ModelRenderActivity: BaseActivity(), AudioRecorder.AudioRecorderCallback {

    private lateinit var modelViewer: ModelViewer
    private val automation = AutomationEngine()
    private val viewerContent = AutomationEngine.ViewerContent()

    private val motionFloatArray by lazy(LazyThreadSafetyMode.NONE) { FloatArray(16) }
    private var modelName = "qianyu"
    private var modelBgName = "venetian_crossroads_2k"
    private var loadStartTime = 0L
    private var loadStartFence: Fence? = null

    private var motionFps = 0.033
    private var isEnableMotion = false
    private var isNeedUpdateMotion = false
    private var currentMotionFileName = "motions/666_girl.json"
    private var motionBean: MotionBean? = null
    private var frameIndex = 0

    lateinit var mMediaRecorder: FFMediaRecorder
    lateinit var mAudioRecorder: AudioRecorder
    var mRecorderType = RenderCons.RECORDER_TYPE_AV

    companion object {
        init {
            Utils.init()
        }

        fun start(context: Context) {
            val intent = Intent(context, ModelRenderActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_model_render

    override fun initData() {
        initModel()

        mMediaRecorder = FFMediaRecorder()
        mMediaRecorder.init(glSurface)

        val treeObserver = textureView.viewTreeObserver
        treeObserver.addOnPreDrawListener(object : ViewTreeObserver.OnPreDrawListener {
            override fun onPreDraw(): Boolean {
                textureView.viewTreeObserver.removeOnPreDrawListener(this)
                LogUtils.i(textureView.measuredWidth, textureView.measuredHeight)
                val ratio = textureView.measuredWidth / textureView.measuredHeight.toFloat()
                glSurface.layoutParams.apply {
                    height = (glSurface.measuredWidth / ratio).toInt()
                    glSurface.layoutParams = this
                }
                return true
            }
        })

        lifecycleScope.launch {
            while (true) {
                val millis = measureTimeMillis { drawFrame() }
                delay((motionFps * 1000L - millis).toLong())
            }
        }
        lifecycleScope.launch(Dispatchers.Default) {
            while (isActive) {
                val millis = measureTimeMillis {
                    val bitmap = textureView.bitmap
                    bitmap?.let {
                        LogUtils.i("onPreviewFrame", bitmap.width, bitmap.height, bitmap.byteCount)
                        val buf = ByteBuffer.allocate(bitmap.byteCount)
                        bitmap.copyPixelsToBuffer(buf)
                        mMediaRecorder.onPreviewFrame(RenderCons.IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
                        mMediaRecorder.requestRender()
                        bitmap.recycle()
                    }
                }
                LogUtils.i(millis)
//                delay((0.04 * 1000L).toLong())
//                delay(40L)
            }
        }

        btnStartRecord.setOnClickListener {
            val frameWidth = if (glSurface.width % 2 == 0) glSurface.width else glSurface.width - 1
            val frameHeight = if (glSurface.height % 2 == 0) glSurface.height else glSurface.height - 1
            LogUtils.i("onClick", glSurface.width, glSurface.height, textureView.width, textureView.height)
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
        mMediaRecorder.setTransformMatrix(0, 0)
    }

    override fun onDestroy() {
        super.onDestroy()
        mMediaRecorder.unInit()
    }

    private fun initModel() {
        modelViewer = ModelViewer(textureView)
        viewerContent.view = modelViewer.view
        viewerContent.sunlight = modelViewer.light
        viewerContent.lightManager = modelViewer.engine.lightManager
        viewerContent.scene = modelViewer.scene
        viewerContent.renderer = modelViewer.renderer

        textureView.setOnTouchListener { _, event ->
            modelViewer.onTouchEvent(event)
            true
        }
        loadGltfModel()
        loadGltfBg()
    }

    private fun loadGltfModel() {
        val buffer = readCompressedAsset("models/${modelName}/${modelName}.gltf")
        modelViewer.loadModelGltf(buffer) { uri -> readCompressedAsset("models/${modelName}/${uri}") }
        updateRootTransform()
    }

    private fun loadGltfBg() {
        val engine = modelViewer.engine
        val scene = modelViewer.scene
        readCompressedAsset("envs/${modelBgName}/${modelBgName}_ibl.ktx").let {
            scene.indirectLight = KTX1Loader.createIndirectLight(engine, it)
            scene.indirectLight!!.intensity = 30_000.0f
            viewerContent.indirectLight = modelViewer.scene.indirectLight
        }
        readCompressedAsset("envs/${modelBgName}/${modelBgName}_skybox.ktx").let {
            scene.skybox = KTX1Loader.createSkybox(engine, it)
        }
    }

    private fun readCompressedAsset(assetName: String): ByteBuffer {
        val input = this.assets.open(assetName)
        val bytes = ByteArray(input.available())
        input.read(bytes)
        return ByteBuffer.wrap(bytes)
    }

    private fun updateRootTransform() {
        if (automation.viewerOptions.autoScaleEnabled) {
            modelViewer.transformToUnitCube()
        } else {
            modelViewer.clearRootTransform()
        }
    }

    fun drawFrame() {
        loadStartFence?.let {
            if (it.wait(Fence.Mode.FLUSH, 0) == Fence.FenceStatus.CONDITION_SATISFIED) {
                val end = System.nanoTime()
                val total = (end - loadStartTime) / 1_000_000
                Log.i("loadStartFence", "The Filament backend took $total ms to load the model geometry.")
                modelViewer.engine.destroyFence(it)
                loadStartFence = null
            }
        }
        if (modelViewer.progress == 1.0f) {
            modelViewer.animator?.apply {
                if (isEnableMotion) {
                    if (isNeedUpdateMotion) {
                        readMotionJson(currentMotionFileName)
                        motionBean?.apply {
                            motionFps = totalTime / curves[0].rotas.size
                        }
                        isNeedUpdateMotion = false
                        frameIndex = 0
                    }
                    setMotionTransforms() // 动作
                }
                updateBoneMatrices() // 更新
            }
        }
        modelViewer.render((motionFps * 1_000_000_000L).toLong())
    }

    private fun readMotionJson(fileName: String) {
        val res = ResourceUtils.readAssets2String(fileName)
        motionBean = GsonUtils.fromJson(res, MotionBean::class.java)
    }

    /**
     * 从本地json文件中获取数据
     */
    private fun setMotionTransforms(){
        val manager = modelViewer.engine.transformManager
        val asset = modelViewer.asset

        motionBean?.curves?.forEachIndexed { _, it ->
            if (frameIndex >= it.rotas.size) frameIndex = 0
            val rota = it.rotas[frameIndex]
            val x = rota.x.toFloat()
            val y = rota.y.toFloat()
            val z = rota.z.toFloat()
            val w = rota.w.toFloat()

            val m00 = 1 - 2 * pow(y, 2f) - 2 * pow(z, 2f);
            val m01 = 2 * x * y - 2 * w * z;
            val m02 = 2 * x * z + 2 * w * y;

            val m10 = 2 * x * y + 2 * w * z;
            val m11 = 1 - 2 * pow(x, 2f) - 2 * pow(z, 2f);
            val m12 = 2 * y * z - 2 * w * x;

            val m20 = 2 * x * z - 2 * w * y;
            val m21 = 2 * y * z + 2 * w * x;
            val m22 = 1 - 2 * pow(x, 2f) - 2 * pow(y, 2f);

            val entity = asset?.getFirstEntityByName(it.name) ?: return@forEachIndexed
            manager.getTransform(entity - 1, motionFloatArray)
            motionFloatArray[0] = m00
            motionFloatArray[1] = -m10
            motionFloatArray[2] = -m20
            motionFloatArray[4] = -m01
            motionFloatArray[5] = m11
            motionFloatArray[6] = m21
            motionFloatArray[8] = -m02
            motionFloatArray[9] = m12
            motionFloatArray[10] = m22
//            Log.e(TAG,name+"-"+frameIndex+"-"+floatArray.contentToString())
            manager.setTransform(entity - 1, motionFloatArray)
        }
        frameIndex++
    }

    private fun getVideoPath(): String {
        return "${externalCacheDir?.path}/mediacodec_demo.mp4"
    }

    override fun onAudioData(data: ByteArray, dataSize: Int) {
        mMediaRecorder.onAudioData(data)
    }

    override fun onError(msg: String) {
        LogUtils.e(msg)
    }

}