package com.chenhbzl.screenshot;

import com.chenhbzl.screenshot.jni.NativeScreenShot;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Button btnShot = (Button)this.findViewById(R.id.button1);
        btnShot.setOnClickListener(new OnClickListener(){

            @Override
            public void onClick(View v) {
                // TODO Auto-generated method stub
                String strFilename = "/sdcard/test_ScreenShot.png";
                int result = NativeScreenShot.screenshot(strFilename);
                Log.e("", "ScreenShot result = " + result);
            }});
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    static {
        System.loadLibrary("screenshot_app");
    }
}
