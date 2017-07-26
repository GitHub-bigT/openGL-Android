package ricoh.opengljava;

/**
 * Created by bigT on 2017/7/26.
 */

public class NativeMethod {
    public static native void onSurfaceCreated();
    public static native void onSurfaceChanged(int width, int height);
    public static native void onDrawFrame();
}
