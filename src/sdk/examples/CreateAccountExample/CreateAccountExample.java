public class CreateAccountExample {
    static {
        System.loadLibrary("hedera-sdk-cpp-create-account-example");
    }

    public static void main(String[] args) {
        new CreateAccountExample().nativeMain();
    }

    public native void nativeMain();
}