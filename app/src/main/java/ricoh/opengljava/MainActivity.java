package ricoh.opengljava;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;


public class MainActivity extends Activity {
    private GLSurfaceView mGLSurfaceView;
    private static final String TAG = "ooopppp";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
    }

    private void init() {
        mGLSurfaceView = (GLSurfaceView) findViewById(R.id.main_gl);
        mGLSurfaceView.setEGLContextClientVersion(2);
        Log.i(TAG,"已设置opengl版本");
        mGLSurfaceView.setRenderer(new MyRender());
        Log.i(TAG,"已设置渲染器");
        mGLSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        Log.i(TAG,"已设置渲染模式");
    }
}
