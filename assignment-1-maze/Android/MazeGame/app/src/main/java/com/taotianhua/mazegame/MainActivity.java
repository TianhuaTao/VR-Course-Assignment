package com.taotianhua.mazegame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("mazeGame-lib");
    }


    private GLSurfaceView gLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getSupportActionBar().hide();

        // Set assets manager
        AssetManager mgr = this.getApplicationContext().getResources().getAssets();
        setAssetManager(mgr);


        setContentView(R.layout.activity_main);

//        gLView = new MazeGameGLSurfaceView(this);
//        setContentView(gLView);
        Button bUp = findViewById(R.id.buttonU);
        Button bDown= findViewById(R.id.buttonD);
        Button bLeft= findViewById(R.id.buttonL);
        Button bRight= findViewById(R.id.buttonR);
        Button bForward= findViewById(R.id.buttonF);
        Button bBack= findViewById(R.id.buttonB);

        MazeGameGLSurfaceView surfaceView = findViewById(R.id.surfaceView);
        MazeGameGLRenderer renderer =  surfaceView.getRenderer();
        renderer.setbUp(bUp);
        renderer.setbDown(bDown);
        renderer.setbLeft(bLeft);
        renderer.setbRight(bRight);
        renderer.setbForward(bForward);
        renderer.setbBack(bBack);


    }


    private  native void setAssetManager(AssetManager assetManager);
}
