package com.app.testopengl.opengl.drawer

import android.graphics.SurfaceTexture

interface IDrawer {
    fun setTextureId(id: Int)
    fun draw()
    fun release()

    fun getSurfaceTexture(cb: (st: SurfaceTexture) -> Unit) {}
}