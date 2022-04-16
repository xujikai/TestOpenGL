package com.app.testopengl.opengl.drawer;

import static android.opengl.GLES20.GL_POINTS;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniform4f;
import static android.opengl.GLES20.glUseProgram;

import android.content.Context;
import android.opengl.GLES20;

import com.app.testopengl.R;
import com.app.testopengl.opengl.BaseDrawer;
import com.app.testopengl.utils.ShaderHelper;
import com.app.testopengl.utils.VertexArray;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class PointDrawer extends BaseDrawer {

    // 着色器中定义的变量，在 Java 层绑定并赋值
    private static final String U_COLOR = "u_Color";
    private static final String A_POSITION = "a_Position";
    private int aColorLocation;
    private int aPositionLocation;

    float[] pointVertex = {
            0f, 0f
    };

    public PointDrawer(Context context) {
        super(context);
        mProgram = ShaderHelper.buildProgram(context, R.raw.point_vert_shader, R.raw.point_frag_shader);

        glUseProgram(mProgram);
        vertexArray = new VertexArray(pointVertex);
        POSITION_COMPONENT_COUNT = 2;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //绑定值
        aColorLocation = glGetUniformLocation(mProgram, U_COLOR);
        aPositionLocation = glGetAttribLocation(mProgram, A_POSITION);
        // 给绑定的值赋值，也就是从顶点数据那里开始读取，每次读取间隔是多少
        vertexArray.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        // 给绑定的值赋值
        glUniform4f(aColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_POINTS, 0, pointVertex.length / POSITION_COMPONENT_COUNT);
    }

    @Override
    public void onDrawFrame(GL10 gl, float[] mvpMatrix) {
        super.onDrawFrame(gl, mvpMatrix);
    }

    @Override
    public void onSurfaceDestroyed() {
        super.onSurfaceDestroyed();
        GLES20.glDeleteProgram(mProgram);
    }

}
