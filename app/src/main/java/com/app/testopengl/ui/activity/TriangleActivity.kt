package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.MyGLRender
import com.app.testopengl.opengl.MyNativeRender
import com.blankj.utilcode.util.ImageUtils
import kotlinx.android.synthetic.main.activity_common_surface.*
import java.nio.ByteBuffer

class TriangleActivity: BaseActivity() {

    val IMAGE_FORMAT_RGBA = 0x01
    val IMAGE_FORMAT_NV21 = 0x02
    val IMAGE_FORMAT_NV12 = 0x03
    val IMAGE_FORMAT_I420 = 0x04
    val IMAGE_FORMAT_YUYV = 0x05
    val IMAGE_FORMAT_GARY = 0x06

    private var mNativeRenderer: MyNativeRender = MyNativeRender()
    private var mGLRender: MyGLRender = MyGLRender(mNativeRenderer)

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, TriangleActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_common_surface

    override fun initData() {
        mGLRender.init()
        loadRGBAImage(R.drawable.texture)
//        loadNV21Image()
        glSurface.setEGLContextClientVersion(3)
        glSurface.setRenderer(mGLRender)
    }

    /**
     * 加载RGBA格式的图片
     */
    private fun loadRGBAImage(resId: Int) {
        val bitmap = ImageUtils.getBitmap(resId)
        val buf = ByteBuffer.allocate(bitmap.byteCount)
        bitmap.copyPixelsToBuffer(buf)
        mGLRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
    }

    /**
     * 加载NV21格式的图片
     */
    private fun loadNV21Image() {
        val inputStream = assets.open("YUV_Image_840x1074.NV21")
        val len = inputStream.available()
        val buf = ByteArray(len)
        inputStream.read(buf)
        mGLRender.setImageData(IMAGE_FORMAT_NV21, 840, 1074, buf)
        inputStream.close()
    }

    override fun onDestroy() {
        super.onDestroy()
        mGLRender.unInit()
    }

}