package ca.bluink.jniexamples;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import java.lang.annotation.Native;

import ca.bluink.jniexamples.Native.NativeInterface;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("native");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        NativeInterface.register();
        int result = NativeInterface.startTest(5);
        Log.d("MAIN", "Result of startTest: " + result);
        NativeInterface.receivedBytesToSign(new byte[]{'S', 'I', 'G', 'N', 'M', 'E', '!'});
    }
}
