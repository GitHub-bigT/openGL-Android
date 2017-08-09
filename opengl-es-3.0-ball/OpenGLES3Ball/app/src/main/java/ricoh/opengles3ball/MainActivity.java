package ricoh.opengles3ball;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends Activity {
    private GLSurfaceView mGL;
    private final int GL_VERSION = 3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mGL = findViewById(R.id.mGL);
        //检测手机是否支持opengl es 3
        if (detectOpenGLES3()) {
            Toast.makeText(this, "cell-phone support opengl es 3", Toast.LENGTH_SHORT).show();
            mGL.setEGLContextClientVersion(GL_VERSION);
            mGL.setRenderer(new MyRender());
        }else {
            Toast.makeText(this, "cell-phone don't support opengl es 3", Toast.LENGTH_SHORT).show();
        }
    }

    private boolean detectOpenGLES3() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 3);
    }
}
