package ricoh.opengljava;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by 田帅 on 2017/7/22.
 */

public class MyRender implements GLSurfaceView.Renderer {
    private static final String TAG = "ooopppp";
    private Triangle mTriangle;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "surface create");
        //背景颜色
//        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        mTriangle = new Triangle();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(TAG, "surface change");
        Log.i(TAG, "width :" + width + " height:" + height);
        GLES20.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        Log.i(TAG, "onDrawFrame");
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        mTriangle.draw();
    }

    /**
     * 画一个三角形
     */
    class Triangle {
        private String vertexShaderCode =
                "attribute vec4 vPosition;" +
                        "void main() {" +
                        "  gl_Position = vPosition;" +
                        "}";
        private String fragmentShaderCode =
                "precision mediump float;" +
                        "uniform vec4 vColor;" +
                        "void main() {" +
                        "  gl_FragColor = vColor;" +
                        "}";
        private int mProgram = GLES20.glCreateProgram();
        private int mPositionHandler;
        private float trianglecoords[] = {
                0.0f, 1f, 0.0f,   //上
                -1f, 0f, 0.0f,   // 左下
                1f, -1f, 0.0f    // 右下
        };
        private FloatBuffer vertexBuffer;
        //数组中每个顶点的坐标数
        int COORDS_PER_VERTEX = 3;
        private final int vertexCount = trianglecoords.length / COORDS_PER_VERTEX;
        private final int vertexStride = COORDS_PER_VERTEX * 4; // bytes per vertex
        private int mColorHandle;
        float color[] = { 1f, 0f, 0f, 1.0f };

        public Triangle() {
            //初始化字节缓冲区
            //为三角形初始化顶点字节缓冲区
            ByteBuffer bb = ByteBuffer.allocateDirect(trianglecoords.length * 4);
//            for (int i = 0; i < trianglecoords.length; i++) {
//                Log.i(TAG,"float[] "+trianglecoords[i]);
//            }
//            for (int i = 0 ; i < bb.array().length ; i++) {
//                Log.i(TAG,"bb "+bb.array()[i]);
//            }
            //修改缓冲区的字节顺序
            //ByteOrder.nativeOrder():分配与硬件相同字节顺序的直接缓冲区，使本机代码库效率更高
            bb.order(ByteOrder.nativeOrder());
//            for (int i = 0 ; i < bb.array().length ; i++) {
//                Log.i(TAG,"bb order  "+bb.array()[i]);
//            }
            Log.i(TAG, "program: " + mProgram);
            vertexBuffer = bb.asFloatBuffer();
            vertexBuffer.put(trianglecoords);
            vertexBuffer.position(0);

//            float[] a = new float[vertexBuffer.limit()];
//            vertexBuffer.get(a);
//            for (int i = 0; i < a.length; i++) {
//                Log.i(TAG, "a " + a[i]);
//            }

            //准备渲染和openGL程序
            int vertexShader = MyRender.loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
            int fragmentShader = MyRender.loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);
            Log.i(TAG, "vertex shader: " + vertexShader);
            Log.i(TAG, "fragment shader: " + fragmentShader);
            mProgram = GLES20.glCreateProgram();
            GLES20.glAttachShader(mProgram, vertexShader);
            GLES20.glAttachShader(mProgram, fragmentShader);
            //创建opengl程序的可执行文件
            GLES20.glLinkProgram(mProgram);
        }

        public void draw() {
            GLES20.glUseProgram(mProgram);
            mPositionHandler = GLES20.glGetAttribLocation(mProgram, "vPosition");
            GLES20.glEnableVertexAttribArray(mPositionHandler);
            GLES20.glVertexAttribPointer(mPositionHandler, COORDS_PER_VERTEX,GLES20.GL_FLOAT,false,vertexStride,vertexBuffer);
            //设置颜色
            mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
            GLES20.glUniform4fv(mColorHandle, 1, color, 0);
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
            // Disable vertex array
            GLES20.glDisableVertexAttribArray(mPositionHandler);
        }
    }

    private static int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        Log.i(TAG, "type: " + type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }

}
