package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.view.Surface
import com.app.testopengl.R
import com.app.testopengl.base.BaseActivity
import com.app.testopengl.media.decoder.AudioDecoder
import com.app.testopengl.media.decoder.VideoDecoder
import com.app.testopengl.opengl.SimpleRender
import com.app.testopengl.opengl.drawer.IDrawer
import com.app.testopengl.opengl.drawer.VideoDrawer
import com.blankj.utilcode.util.PathUtils
import kotlinx.android.synthetic.main.activity_opengl_player.*
import java.util.concurrent.Executors

class OpenGLPlayerActivity : BaseActivity() {

    private val path = PathUtils.getExternalStoragePath() + "/car.mp4"
    lateinit var drawer: IDrawer

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, OpenGLPlayerActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun getLayoutId(): Int = R.layout.activity_opengl_player

    override fun initData() {
        drawer = VideoDrawer()
        drawer.getSurfaceTexture {
            initPlayer(Surface(it))
        }
        glSurface.setEGLContextClientVersion(2)
        glSurface.setRenderer(SimpleRender(drawer))
    }

    private fun initPlayer(sf: Surface) {
        val threadPool = Executors.newFixedThreadPool(10)

        val videoDecoder = VideoDecoder(path, null, sf)
        threadPool.execute(videoDecoder)

        val audioDecoder = AudioDecoder(path)
        threadPool.execute(audioDecoder)

        videoDecoder.goOn()
        audioDecoder.goOn()
    }

}