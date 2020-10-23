package com.taotianhua.maze_vr;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("MazeVR_jni");
    }
    private static final String TAG = MainActivity.class.getSimpleName();
    private GLSurfaceView glView;
    private long nativeApp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nativeApp = nativeOnCreate(getAssets());
        setContentView(R.layout.activity_main);
        glView = findViewById(R.id.surface_view);
        glView.setEGLContextClientVersion(2);
        Renderer renderer = new Renderer();
        glView.setRenderer(renderer);
        glView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        glView.setOnTouchListener(
                (v, event) -> {
                    if (event.getAction() == MotionEvent.ACTION_DOWN) {
                        // Signal a trigger event.
                        glView.queueEvent(
                                () -> {
                                    nativeOnTriggerEvent(nativeApp);
                                });
                        return true;
                    }
                    return false;
                });

    }
    private class Renderer implements GLSurfaceView.Renderer {
        @Override
        public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
            nativeOnSurfaceCreated(nativeApp);
        }

        @Override
        public void onSurfaceChanged(GL10 gl10, int width, int height) {
            nativeSetScreenParams(nativeApp, width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl10) {
            nativeOnDrawFrame(nativeApp);
        }
    }

    private native long nativeOnCreate(AssetManager assetManager);

    private native void nativeOnDestroy(long nativeApp);

    private native void nativeOnSurfaceCreated(long nativeApp);

    private native void nativeOnDrawFrame(long nativeApp);

    private native void nativeOnTriggerEvent(long nativeApp);

    private native void nativeOnPause(long nativeApp);

    private native void nativeOnResume(long nativeApp);

    private native void nativeSetScreenParams(long nativeApp, int width, int height);

    private native void nativeSwitchViewer(long nativeApp);
}