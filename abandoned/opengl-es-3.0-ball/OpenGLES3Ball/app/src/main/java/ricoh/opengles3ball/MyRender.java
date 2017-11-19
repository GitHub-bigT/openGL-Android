package ricoh.opengles3ball;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by bigT on 2017/8/9.
 */

public class MyRender implements GLSurfaceView.Renderer{
    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {
        NativeMethod.init(i,i1);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        NativeMethod.step();
    }
}
