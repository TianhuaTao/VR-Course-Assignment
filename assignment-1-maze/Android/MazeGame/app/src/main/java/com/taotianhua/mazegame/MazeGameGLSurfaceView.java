package com.taotianhua.mazegame;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MazeGameGLSurfaceView extends GLSurfaceView {
    public MazeGameGLRenderer getRenderer() {
        return renderer;
    }

    private final MazeGameGLRenderer renderer;
    public MazeGameGLSurfaceView(Context context, AttributeSet attrs){
        super(context,attrs);

        // Create an OpenGL ES 3.0 context
        setEGLContextClientVersion(3);

        renderer = new MazeGameGLRenderer();

        // Set the Renderer for drawing on the GLSurfaceView
        setRenderer(renderer);
    }


}
