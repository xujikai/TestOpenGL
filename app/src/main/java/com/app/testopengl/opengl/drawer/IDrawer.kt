package com.app.testopengl.opengl.drawer

import android.graphics.SurfaceTexture

interface IDrawer {
    fun setTextureId(id: Int)
    fun draw()
    fun release()

    fun getSurfaceTexture(cb: (st: SurfaceTexture) -> Unit) {}
    fun setVideoSize(width: Int, height: Int) {}
    fun setWorldSize(width: Int, height: Int) {}
}