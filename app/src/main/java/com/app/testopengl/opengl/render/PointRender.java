package com.app.testopengl.opengl.render;

import static android.opengl.GLES20.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES20.glClear;
import static android.opengl.GLES20.glClearColor;
import static android.opengl.GLES20.glViewport;

import android.content.Context;
import android.opengl.GLSurfaceView;

import com.app.testopengl.opengl.drawer.PointDrawer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class PointRender implements GLSurfaceView.Renderer {
    private Context mContext;
    private PointDrawer mPointDrawer;

    public PointRender(Context context) {
        mContext = context;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        // 在 onSurfaceCreated 里面初始化，否则会报线程错误
        mPointDrawer = new PointDrawer(mContext);
        // 绑定相应的顶点数据
        mPointDrawer.onSurfaceCreated(gl, config);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // 清屏
        glClear(GL_COLOR_BUFFER_BIT);
        // 绘制
        mPointDrawer.onDrawFrame(gl);
    }
}
