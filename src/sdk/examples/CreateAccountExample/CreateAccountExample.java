public class CreateAccountExample {
    static {
        System.loadLibrary("native");
    }

    public static void main(String[] args) {
        new CreateAccountExample().nativeMain();
    }

    public native void nativeMain();
}