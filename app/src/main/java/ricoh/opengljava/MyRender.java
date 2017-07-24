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
    private Triangle mTriangle1;
    private Triangle mTriangle2;
    private Triangle mTriangle3;
    float[] coord1 = {
            0f, 1f, 0.0f,   //上
            -0.7f, 0f, 0.0f,   // 左下
            0.7f, 0f, 0.0f    // 右下
    };
    float[] color1 = { 1f, 0f, 0f ,0f};
    float[] coord2 = {
            0f, 1f, 0.0f,   //上
            0.7f, 0f, 0.0f,   // 左下
            0.9f, 0.3f, 0.0f    // 右下
    };
    float[] color2 = { 0f, 1f, 0f ,0f};
    float[] coord3 = {
            0f, 1f, 0.0f,   //上
            -0.7f, 0f, 0.0f,   // 左下
            -0.5f, 0.3f, 0.0f    // 右下
    };
    float[] color3 = { 0f, 0f, 1f ,0f};
    //当创建GLSurfaceView时被调用，只调用一次．
    // 在这个方法中执行只发生一次的动作，比如设置OpenGL环境参数或初始化OpenGL图形对象
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.i(TAG, "surface create");
        //背景颜色
//        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        mTriangle1 = new Triangle();
        mTriangle2 = new Triangle();
        mTriangle3 = new Triangle();
    }

    //当GLSurfaceView几何体改变时系统调用此方法，
    //比如GLSurfaceView的大小改变或设备屏幕的方向改变．
    // 使用此方法来响应GLSurfaceView容器的变化
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.i(TAG, "surface change");
        Log.i(TAG, "width :" + width + " height:" + height);
//        GLES20.glViewport(0, 0, width, height);
    }

    //系统在每次重绘GLSurfaceView时调用此方法．此方法是绘制图形对象的主要的执行点
    @Override
    public void onDrawFrame(GL10 gl) {
        Log.i(TAG, "onDrawFrame");
//        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        mTriangle1.draw(coord1,color1);
        mTriangle2.draw(coord2,color2);
        mTriangle3.draw(coord3,color3);
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
        private float trianglecoords[] ;
        private FloatBuffer vertexBuffer;
        //数组中每个顶点的坐标数
        int COORDS_PER_VERTEX = 3;
        private final int vertexStride = COORDS_PER_VERTEX * 4; // bytes per vertex
        private int mColorHandle;
        float color[] ;


        public void draw(float coords[],float c[]) {
            this.trianglecoords  = coords;
            this.color = c;
            int vertexCount = trianglecoords.length / COORDS_PER_VERTEX;
            /*
            //为三角形初始化顶点字节缓冲区
            //不能直接使用FloatBuffer ： Must use a native order direct Buffer
            //GLES20.glVertexAttribPointer(mPositionHandler, COORDS_PER_VERTEX,GLES20.GL_FLOAT,false,vertexStride,vertexBuffer);
            //最后一个参数有限制
            */
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
//            vertexBuffer = FloatBuffer.allocate(trianglecoords.length);

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
//            mProgram = GLES20.glCreateProgram();
            GLES20.glAttachShader(mProgram, vertexShader);
            GLES20.glAttachShader(mProgram, fragmentShader);
            //创建opengl程序的可执行文件/
            GLES20.glLinkProgram(mProgram);

            GLES20.glUseProgram(mProgram);
            mPositionHandler = GLES20.glGetAttribLocation(mProgram, "vPosition");
            Log.i(TAG , "mPositionHandler: "+mPositionHandler);
            GLES20.glEnableVertexAttribArray(mPositionHandler);
            GLES20.glVertexAttribPointer(mPositionHandler, COORDS_PER_VERTEX,GLES20.GL_FLOAT,true,vertexStride,vertexBuffer);
            //设置颜色
            mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
            Log.i(TAG , "mColorHandle: "+mColorHandle);
            GLES20.glUniform4fv(mColorHandle, 1, color, 0);
//            GLES20.glUniform3fv(mColorHandle, 1, color, 0);
            GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, vertexCount);
            // Disable vertex array
//            GLES20.glDisableVertexAttribArray(mPositionHandler);
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
