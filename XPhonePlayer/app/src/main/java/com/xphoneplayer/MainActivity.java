package com.xphoneplayer;


import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements Runnable,SeekBar.OnSeekBarChangeListener {

    // Used to load the 'xphoneplayer' library on application startup.
    static {
        System.loadLibrary("xphoneplayer");
    }
    private Button btn;
    private SeekBar seek;
    private Thread th;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //去掉标题栏
        supportRequestWindowFeature( Window.FEATURE_NO_TITLE);
        //全屏，隐藏状态
        getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN ,
                WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        //屏幕为横屏
        setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );

        setContentView( R.layout.activity_main );

        btn = findViewById(R.id.open_button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.e("XPlay", "open button clicked");
                //打开选择路径窗口自
                Intent intent = new Intent();
                intent.setClass(MainActivity.this, OpenUrl.class);
                startActivity(intent);
            }
        });

        seek = findViewById(R.id.aplayseek);
        seek.setMax(1000);
        seek.setOnSeekBarChangeListener(this);

        //启动播放进度
        th = new Thread(this);
        th.start();
    }

    //播放进度显示
    @Override
    public void run() {
        while (true)
        {
            seek.setProgress((int)PlayPos()*1000);
            try {
                Thread.sleep(40);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public native double PlayPos();
    public native void Seek(double pos);

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        Seek((double)seek.getProgress()/(double) seek.getMax());
    }
}