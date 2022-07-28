package com.app.testopengl.utils;

import android.opengl.GLES20;
import android.util.Log;

import com.blankj.utilcode.util.LogUtils;
import com.blankj.utilcode.util.Utils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

public class GLComUtils {

    private static final String TAG = GLComUtils.class.getSimpleName();

    public static int createProgram(int vertexShaderSource, int fragmentShaderSource) {
        String vertexString = TextResourceReader.readTextFileFromResource(Utils.getApp(), vertexShaderSource);
        String textureString = TextResourceReader.readTextFileFromResource(Utils.getApp(), fragmentShaderSource);
        return createProgram(vertexString, textureString);
    }

    public static int createProgram(String vertexSource, String fragmentSource) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return 0;
        }
        int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) {
            return 0;
        }

        int program = GLES20.glCreateProgram();
        checkGlError("glCreateProgram");
        if (program == 0) {
            Log.e(TAG, "Could not create program");
        }
        GLES20.glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        GLES20.glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        GLES20.glLinkProgram(program);
        int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
        if (linkStatus[0] != GLES20.GL_TRUE) {
            Log.e(TAG, "Could not link program: ");
            Log.e(TAG, GLES20.glGetProgramInfoLog(program));
            GLES20.glDeleteProgram(program);
            program = 0;
        }
        return program;
    }

    public static int createProgramFromAssetFile(String vertexFileName, String fragmentFileName) {
        String vertexString = TextResourceReader.readTextFileFromAsset(Utils.getApp(), vertexFileName);
        String fragmentString = TextResourceReader.readTextFileFromAsset(Utils.getApp(), fragmentFileName);

        LogUtils.d("vertex is " + vertexString + " frag is " + fragmentString);
        return createProgram(vertexString, fragmentString);
    }

    public static int loadShader(int shaderType, String source) {
        int shader = GLES20.glCreateShader(shaderType);
        checkGlError("glCreateShader type=" + shaderType);
        GLES20.glShaderSource(shader, source);
        GLES20.glCompileShader(shader);
        int[] compiled = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
        if (compiled[0] == 0) {
            Log.e(TAG, "Could not compile shader " + shaderType + ":");
            Log.e(TAG, " " + GLES20.glGetShaderInfoLog(shader));
            GLES20.glDeleteShader(shader);
            shader = 0;
        }
        return shader;
    }

    public static void checkGlError(String op) {
        int error = GLES20.glGetError();
        if (error != GLES20.GL_NO_ERROR) {
            String msg = op + ": glError 0x" + Integer.toHexString(error);
            Log.e(TAG, msg);
            throw new RuntimeException(msg);
        }
    }

    public static FloatBuffer createFloatBuffer(float[] coords) {
        // Allocate a direct ByteBuffer, using 4 bytes per float, and copy coords into it.
        ByteBuffer bb = ByteBuffer.allocateDirect(coords.length * Constants.BYTES_PRE_FLOAT);
        bb.order(ByteOrder.nativeOrder());
        FloatBuffer fb = bb.asFloatBuffer();
        fb.put(coords);
        fb.position(0);
        return fb;
    }

    public static IntBuffer createIntBuffer(int[] coords) {
        // Allocate a direct ByteBuffer, using 4 bytes per float, and copy coords into it.
        ByteBuffer bb = ByteBuffer.allocateDirect(coords.length * Constants.BYTES_PRE_INT);
        bb.order(ByteOrder.nativeOrder());
        IntBuffer fb = bb.asIntBuffer();
        fb.put(coords);
        fb.position(0);
        return fb;
    }

    public static ByteBuffer createByteBuffer(byte[] coords) {
        // Allocate a direct ByteBuffer, using 4 bytes per float, and copy coords into it.
        ByteBuffer bb = ByteBuffer.allocateDirect(coords.length * Constants.BYTES_PRE_BYTE);
        bb.order(ByteOrder.nativeOrder());
        bb.put(coords);
        bb.position(0);
        return bb;
    }

    private float[] getIdentity() {
        return new float[]{
                1f, 0f, 0f, 0f,
                0f, 1f, 0f, 0f,
                0f, 0f, 1f, 0f,
                0f, 0f, 0f, 1f
        };
    }

}
