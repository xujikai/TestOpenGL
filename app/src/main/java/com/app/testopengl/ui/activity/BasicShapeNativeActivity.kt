package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.MyGLRender
import com.blankj.utilcode.util.ImageUtils
import kotlinx.android.synthetic.main.activity_common_surface.*
import java.nio.ByteBuffer

class BasicShapeNativeActivity: BaseActivity() {

    val IMAGE_FORMAT_RGBA = 0x01
    val IMAGE_FORMAT_NV21 = 0x02
    val IMAGE_FORMAT_NV12 = 0x03
    val IMAGE_FORMAT_I420 = 0x04
    val IMAGE_FORMAT_YUYV = 0x05
    val IMAGE_FORMAT_GARY = 0x06

    private var mGLRender: MyGLRender = MyGLRender()

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, BasicShapeNativeActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_common_surface

    override fun initData() {
        mGLRender.init()
        loadRGBAImage(R.drawable.texture)
//        loadNV21Image("YUV_Image_840x1074.NV21")
//        loadRGBAImage(R.drawable.board_texture, 0)
//        loadRGBAImage(R.drawable.floor, 1)
//        loadRGBAImage(R.drawable.window, 2)
        // 3D头像
//        loadRGBAImage(R.drawable.avatar_a, 0)
//        loadRGBAImage(R.drawable.avatar_b, 1)
//        loadRGBAImage(R.drawable.avatar_c, 2)
        glSurface.setEGLContextClientVersion(3)
        glSurface.setRenderer(mGLRender)
    }

    override fun onDestroy() {
        super.onDestroy()
        mGLRender.unInit()
    }

    /**
     * 加载RGBA格式的图片
     */
//    Bitmap的分辨率会进行放大
//    private fun loadRGBAImage(resId: Int) {
//        val bitmap = ImageUtils.getBitmap(resId)
//        val buf = ByteBuffer.allocate(bitmap.byteCount)
//        bitmap.copyPixelsToBuffer(buf)
//        mGLRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
//    }
//    Bitmap的分辨率不会进行放大
    private fun loadRGBAImage(resId: Int) {
        val inputStream = resources.openRawResource(resId)
        val bitmap: Bitmap = BitmapFactory.decodeStream(inputStream)
        val buf = ByteBuffer.allocate(bitmap.byteCount)
        bitmap.copyPixelsToBuffer(buf)
        mGLRender.setImageData(IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
        inputStream.close()
    }

    private fun loadRGBAImage(resId: Int, index: Int) {
        val inputStream = resources.openRawResource(resId)
        val bitmap: Bitmap = BitmapFactory.decodeStream(inputStream)
        val buf = ByteBuffer.allocate(bitmap.byteCount)
        bitmap.copyPixelsToBuffer(buf)
        mGLRender.setImageDataWithIndex(index, IMAGE_FORMAT_RGBA, bitmap.width, bitmap.height, buf.array())
        inputStream.close()
    }

    /**
     * 加载NV21格式的图片
     */
    private fun loadNV21Image(filePath: String) {
        val inputStream = assets.open(filePath)
        val len = inputStream.available()
        val buf = ByteArray(len)
        inputStream.read(buf)
        mGLRender.setImageData(IMAGE_FORMAT_NV21, 840, 1074, buf)
        inputStream.close()
    }

}