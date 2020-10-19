/**
 * @author T. Tao
 * This is the overall wrapper class for the maze game.
 */

#ifndef MAZEGAME_MAZEGAME_H
#define MAZEGAME_MAZEGAME_H

#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#include "glm/glm.hpp"
#include <jni.h>
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>
#define BUFFER_OFFSET(a) ((void *)(a))
class Camera;
extern Camera camera;
extern glm::vec3 pointLightPositions[];

class MazeGame {

public:
    static void  Init();
    static void  Destroy();
    static MazeGame*  getInstance();

    static void SetAssetManager(AAssetManager * assetManager);

    MazeGame();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();

    void Initialize();

    int GetWindowWidth() {
        return window_width;
    }
    int GetWindowHeight() {
        return window_height;
    }

    void ProcessInput(bool up,bool down, bool left, bool right,
                      bool forward,
                      bool back )
;
private:
    static MazeGame* sInstance;
    int window_width = 0;
    int window_height = 0;
    static AAssetManager *  asset_manager;
public:
    static AAssetManager *getAssetManager();
};


#endif //MAZEGAME_MAZEGAME_H
