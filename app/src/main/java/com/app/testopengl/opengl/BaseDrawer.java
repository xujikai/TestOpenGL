package com.app.testopengl.opengl;

import android.content.Context;
import android.opengl.GLES20;

import com.app.testopengl.utils.VertexArray;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public abstract class BaseDrawer {

    protected Context mContext;

    protected VertexArray vertexArray;
    protected VertexArray indexArray;

    protected int mProgram;

    protected float[] mModelMatrix = new float[16];
    protected float[] mViewMatrix = new float[16];
    protected float[] mProjectionMatrix = new float[16];
    protected float[] mMvpMatrix = new float[16];

    protected int POSITION_COMPONENT_COUNT;
    protected int TEXTURE_COORDINATES_COMPONENT_COUNT = 2;
    protected int STRIDE;

    public BaseDrawer(Context context) {
        mContext = context;
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    public void onDrawFrame(GL10 gl) {
        GLES20.glClearColor(0f, 0f, 0f, 1f);
        GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
    }

    public void onDrawFrame(GL10 gl, float[] mvpMatrix) {
        GLES20.glClearColor(0f, 0f, 0f, 1f);
        GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
    }

    public void onSurfaceDestroyed() {

    }

}
