package ricoh.opengles3;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends Activity {

    private final int CONTEXT_CLIENT_VERSION = 3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GLSurfaceView view = new GLSurfaceView(this);
//        view.setEGLContextClientVersion(3);
//        view.setRenderer(new MyRender());

        if (detectOpenGLES30()) {
            view.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);
            view.setRenderer(new MyRender());
        } else {
            Log.e("bigT", "OpenGL ES 3.0 not supported on device.  Exiting...");
            finish();
        }

        setContentView(view);
    }

    private boolean detectOpenGLES30() {
        ActivityManager am = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();

        return (info.reqGlEsVersion >= 0x30000);
    }

}
