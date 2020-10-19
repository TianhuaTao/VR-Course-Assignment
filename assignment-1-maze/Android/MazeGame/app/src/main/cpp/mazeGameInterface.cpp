/**
 * @author T. Tao
 * This is the overall JNI interface of mazeGame-lib.
 * The c++ code here is called by Java code, working as hooks.
 */

#include <jni.h>
#include <string>
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>
#include <iostream>
#include "MazeGame.h"
extern "C"
JNIEXPORT void JNICALL
Java_com_taotianhua_mazegame_MazeGameGLRenderer_onSurfaceCreated(JNIEnv *env, jobject thiz,
                                                                 jobject gl10, jobject egl_config) {
    MazeGame::Init();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_taotianhua_mazegame_MazeGameGLRenderer_onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                 jobject gl10, jint width,
                                                                 jint height) {
    MazeGame::getInstance()->onSurfaceChanged(width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_taotianhua_mazegame_MazeGameGLRenderer_onDrawFrameNative(JNIEnv *env, jobject thiz,
                                                                  jobject gl10) {
    MazeGame::getInstance()->onDrawFrame();
}extern "C"
JNIEXPORT void JNICALL
Java_com_taotianhua_mazegame_MainActivity_setAssetManager(JNIEnv *env, jobject thiz,
                                                          jobject asset_manager) {
    AAssetManager *nativeasset = AAssetManager_fromJava(env, asset_manager);
    MazeGame::SetAssetManager(nativeasset);
}extern "C"
JNIEXPORT void JNICALL
Java_com_taotianhua_mazegame_MazeGameGLRenderer_processInputNative(JNIEnv *env, jobject thiz,
                                                                   jboolean up, jboolean down,
                                                                   jboolean left, jboolean right,
                                                                   jboolean forward,
                                                                   jboolean back) {
    MazeGame::getInstance()->ProcessInput(up,down,left,right, forward,back);
}