package ricoh.opengles3;

/**
 * Created by bigT on 2017/8/1.
 */

public class NativeMethod {
    static {
        System.loadLibrary("interface");
    }
    public native static void init(int width , int height);
    public native static void step();
}
