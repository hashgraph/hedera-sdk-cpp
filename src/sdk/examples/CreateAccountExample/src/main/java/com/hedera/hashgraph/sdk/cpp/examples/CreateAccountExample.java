package com.hedera.hashgraph.sdk.cpp.examples;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class CreateAccountExample extends Activity {
    static {
        System.loadLibrary("hedera-sdk-cpp-create-account-example");
    }

    public native void createAccountExample();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        createAccountExample();
    }
}