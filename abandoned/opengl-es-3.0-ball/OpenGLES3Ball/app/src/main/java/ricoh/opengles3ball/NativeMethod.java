package ricoh.opengles3ball;

/**
 * Created by bigT on 2017/8/9.
 */

public class NativeMethod {
    static {
        System.loadLibrary("main");
    }
    public static native void init(int width , int weight);
    public static native void step();
}
