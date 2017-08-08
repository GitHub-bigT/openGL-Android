package ricoh.opengles3;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    private GLSurfaceView mGL;
    private Button mBtn;

    private final int CONTEXT_CLIENT_VERSION = 3;

    int Triangles = 1;
    int OtherTriangles = 2;
    int current;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mGL = findViewById(R.id.mGL);
        mBtn = findViewById(R.id.mBtn);
        mBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (current == Triangles) {
                    current = OtherTriangles;
                }else {
                    current = Triangles;
                }
                    NativeMethod.switchVAO(current);
            }
        });

        if (detectOpenGLES30()) {
            mGL.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION);
            mGL.setRenderer(new MyRender());
        } else {
            Log.e("bigT", "OpenGL ES 3.0 not supported on device.  Exiting...");
            finish();
        }
    }

    private boolean detectOpenGLES30() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x30000);
    }

}
