package com.app.testopengl.opengl.drawer;

import static android.opengl.GLES20.GL_LINES;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniform4f;
import static android.opengl.GLES20.glUniformMatrix4fv;
import static android.opengl.GLES20.glUseProgram;

import android.content.Context;

import com.app.testopengl.R;
import com.app.testopengl.opengl.BaseDrawer;
import com.app.testopengl.utils.ShaderHelper;
import com.app.testopengl.utils.VertexArray;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class LineDrawer extends BaseDrawer {

    private static final String U_COLOR = "u_Color";
    private static final String A_POSITION = "a_Position";
    private static final String U_MODEL_MATRIX = "u_ModelMatrix"; // 模型矩阵
    private static final String U_VIEW_MATRIX = "u_ViewMatrix"; // 视图矩阵
    private static final String U_PROJECTION_MATRIX = "u_ProjectionMatrix"; // 投影矩阵

    private int aColorLocation;
    private int aPositionLocation;
//    private int uProjectionMatrixLocation;
//    private int uViewMatrixLocation;
//    private int uMatrixLocation;

    float[] pointVertex = {
            -0.5f, 0.5f,
            0.5f, -0.5f
    };

    public LineDrawer(Context context) {
        super(context);
        mProgram = ShaderHelper.buildProgram(context, R.raw.line_vert_shader, R.raw.line_frag_shader);

        glUseProgram(mProgram);
        vertexArray = new VertexArray(pointVertex);
        POSITION_COMPONENT_COUNT = 2;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        aColorLocation = glGetUniformLocation(mProgram, U_COLOR);
        aPositionLocation = glGetAttribLocation(mProgram, A_POSITION);
//        uMatrixLocation = glGetUniformLocation(mProgram, U_MODEL_MATRIX);
//        uProjectionMatrixLocation = glGetUniformLocation(mProgram, U_PROJECTION_MATRIX);
//        uViewMatrixLocation = glGetUniformLocation(mProgram, U_VIEW_MATRIX);

        vertexArray.setVertexAttribPointer(0, aPositionLocation, POSITION_COMPONENT_COUNT, 0);

//        setIdentityM(modelMatrix, 0);
//        setIdentityM(viewMatrix, 0);
//        Matrix.translateM(modelMatrix, 0, 0.5f, 0, 0);
//        Matrix.setLookAtM(viewMatrix, 0, 0f, 0f, 10f, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
//        Matrix.perspectiveM(projectionMatrix, 0, 5f, (float) width / (float) height, 9f, 20f);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        super.onDrawFrame(gl);

        glUniform4f(aColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);

        // 使用矩阵平移，将坐标 x 轴平移 0.5 个单位
//        glUniformMatrix4fv(uMatrixLocation, 1, false, modelMatrix, 0);
//        glUniformMatrix4fv(uProjectionMatrixLocation, 1, false, projectionMatrix, 0);
//        glUniformMatrix4fv(uViewMatrixLocation, 1, false, viewMatrix, 0);

        glDrawArrays(GL_LINES, 0, 2);
    }
}
