package com.taotianhua.mazegame;

import android.opengl.GLSurfaceView;
import android.widget.Button;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MazeGameGLRenderer implements GLSurfaceView.Renderer {


    @Override
    public native void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig);

    @Override
    public native void onSurfaceChanged(GL10 gl10, int width, int height) ;


    @Override
    public void onDrawFrame(GL10 gl10) {
        processInput();
        onDrawFrameNative(gl10);
    }

    public native void onDrawFrameNative(GL10 gl10) ;

    private void processInput(){
        boolean up,down,left, right, forward, back;
        up = bUp.isPressed();
        down = bDown.isPressed();
        left = bLeft.isPressed();
        right = bRight.isPressed();
        forward = bForward.isPressed();
        back = bBack.isPressed();
        processInputNative(up,down,left,right,forward,back);
    }

    private native void processInputNative(boolean up,boolean down,boolean left,boolean right,boolean forward,boolean back);


    Button bUp ;
    Button bDown;
    Button bLeft;
    Button bRight;

    public void setbUp(Button bUp) {
        this.bUp = bUp;
    }

    public void setbDown(Button bDown) {
        this.bDown = bDown;
    }

    public void setbLeft(Button bLeft) {
        this.bLeft = bLeft;
    }

    public void setbRight(Button bRight) {
        this.bRight = bRight;
    }

    public void setbForward(Button bForward) {
        this.bForward = bForward;
    }

    public void setbBack(Button bBack) {
        this.bBack = bBack;
    }

    Button bForward;
    Button bBack;
}
