package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.opengl.GLES20
import android.opengl.GLException
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.opengl.MyNativeEglRender
import kotlinx.android.synthetic.main.activity_egl.*
import java.io.IOException
import java.nio.ByteBuffer
import java.nio.IntBuffer

class EglActivity: BaseActivity() {

    private var mEglRender: MyNativeEglRender = MyNativeEglRender()

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, EglActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_egl

    override fun initData() {
        mEglRender.nativeEglInit()

        btnExec.setOnClickListener {
            if (btnExec.text.equals("重置")) {
                ivImage.setImageResource(R.drawable.leg)
                btnExec.text = "后台渲染"
            } else {
                startBgRender()
                btnExec.text = "重置"
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        mEglRender.nativeEglUnInit()
    }

    private fun startBgRender() {
        loadRGBAImage(R.drawable.leg)
        mEglRender.nativeEglDraw()
        ivImage.setImageBitmap(createBitmapFromGLSurface(0, 0, 933, 1400))
//        ivImage.setImageBitmap(createBitmapFromGLSurface(0, 0, 2566, 3850))
    }

    /**
     * 加载RGBA格式的图片
     */
    private fun loadRGBAImage(resId: Int) {
        val inputStream = this.resources.openRawResource(resId)
        val bitmap: Bitmap?
        try {
            bitmap = BitmapFactory.decodeStream(inputStream)
            if (bitmap != null) {
                val bytes = bitmap.byteCount
                val buf = ByteBuffer.allocate(bytes)
                bitmap.copyPixelsToBuffer(buf)
                val byteArray = buf.array()
                mEglRender.nativeEglSetImageData(bitmap.width, bitmap.height, byteArray)
            }
        } finally {
            try {
                inputStream.close()
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }
//    private fun loadRGBAImage(resId: Int) {
//        val bitmap = ImageUtils.getBitmap(resId)
//        val buf = ByteBuffer.allocate(bitmap.byteCount)
//        bitmap.copyPixelsToBuffer(buf)
//        mEglRender.nativeEglSetImageData(bitmap.width, bitmap.height, buf.array())
//    }

    private fun createBitmapFromGLSurface(x: Int, y: Int, w: Int, h: Int): Bitmap? {
        val bitmapBuffer = IntArray(w * h)
        val bitmapSource = IntArray(w * h)
        val intBuffer = IntBuffer.wrap(bitmapBuffer)
        intBuffer.position(0)
        try {
            GLES20.glReadPixels(x, y, w, h, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, intBuffer)
            var offset1 = 0
            var offset2 = 0
            for (i in 0 until h) {
                offset1 = i * w
                offset2 = (h - i - 1) * w
                for (j in 0 until w) {
                    val texturePixel: Int = bitmapBuffer[offset1 + j]
                    val blue: Int = (texturePixel shr 16) and 0xff
                    val red: Int = (texturePixel shl 16) and 0x00ff0000
                    val pixel: Int = (texturePixel and 0xff00ff00.toInt()) or red or blue
                    bitmapSource[offset2 + j] = pixel
                }
            }
        } catch (e: GLException) {
            return null
        }
        return Bitmap.createBitmap(bitmapSource, w, h, Bitmap.Config.ARGB_8888)
    }

}