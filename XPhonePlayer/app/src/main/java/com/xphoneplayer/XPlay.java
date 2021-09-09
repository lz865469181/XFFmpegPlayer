package com.xphoneplayer;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class XPlay extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer, View.OnClickListener {

    public XPlay(Context context, AttributeSet attrs) {
        super(context, attrs);
        //android 8.0需要设置
        setRenderer(this);
        setOnClickListener(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder)
    {
        //初始化opengl egl 显示
        InitView(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder var1, int var2, int var3, int var4)
    {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder var1)
    {

    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {

    }

    @Override
    public void onDrawFrame(GL10 var1)
    {

    }

    public native void InitView(Object surface);
    public native void PlayOrPause();

    @Override
    public void onClick(View view) {
        PlayOrPause();
    }
}
