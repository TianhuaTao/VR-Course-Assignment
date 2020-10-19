//
// Created by tth13 on 9/25/2020.
//

#include "MazeGame.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#include <GLES3/gl3ext.h>
#include "Shader.h"
#include "ContainerBox.h"

MazeGame* MazeGame::sInstance;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
void MazeGame::Init() {
    if (sInstance)
        return;
    sInstance = new MazeGame;
    sInstance->Initialize();
}

MazeGame *MazeGame::getInstance() {
    return sInstance;
}

void MazeGame::onSurfaceChanged(int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}

void MazeGame::Destroy() {
    delete  sInstance;
    sInstance = nullptr;
}

MazeGame::MazeGame() {

}

void MazeGame::Initialize() {
    // configure global opengl state
    glEnable(GL_DEPTH_TEST);


    ContainerBox::Initialize();

}

void MazeGame::onDrawFrame() {
    static const float background_color[] = { 0.49f, 0.73f, 0.92f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, background_color);
    glClear(GL_DEPTH_BUFFER_BIT);
    ContainerBox::draw();
}

void MazeGame::SetAssetManager(AAssetManager * assetManager) {
    MazeGame::asset_manager = assetManager;
}

AAssetManager *MazeGame::getAssetManager() {
    return asset_manager;
}

// timing
double deltaTime = 0.0f;
float lastFrame = 0.0f;

double calculateElapsedTime(){
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    static TimePoint tp1 = std::chrono::high_resolution_clock::now();
    auto tp2 = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1);
    tp1 = tp2;
    return dur.count()/1000.0;
}

void MazeGame::ProcessInput(bool up, bool down, bool left, bool right, bool forward, bool back) {
    deltaTime = calculateElapsedTime();

    if (forward)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (back)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (left)
//        camera.ProcessKeyboard(LEFT, deltaTime);
        camera.ProcessMouseMovement(-10,0);
    if (right)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
        camera.ProcessMouseMovement(10,0);
    if(up)
        camera.ProcessMouseMovement(0,10);
        if(down)
            camera.ProcessMouseMovement(0,-10);
}

AAssetManager *  MazeGame::asset_manager;