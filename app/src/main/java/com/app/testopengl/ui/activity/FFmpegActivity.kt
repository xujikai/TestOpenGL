package com.app.testopengl.ui.activity

import android.content.Context
import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.app.testopengl.R
import com.app.testopengl.ffmpeg.MyNativeMedia
import kotlinx.android.synthetic.main.activity_ffmpeg.*

class FFmpegActivity: AppCompatActivity() {

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, FFmpegActivity::class.java)
            context.startActivity(intent)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_ffmpeg)
        val mNativeMedia = MyNativeMedia()
        tvVersion.text = mNativeMedia.nativeGetFFmpegVersion()
    }

}