/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_
#define HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_

#include <android/asset_manager.h>
#include <jni.h>

#include "glm/glm.hpp"

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <GLES2/gl2.h>
#include "cardboard.h"
#include "util.h"


extern glm::vec3 pointLightPositions[];

class Camera;

extern Camera camera;

/**
 * This is a sample app for the Cardboard SDK. It loads a simple environment and
 * objects that you can click on.
 */
class MazeGame {
public:
    /**
     * Creates a MazeGame.
     *
     * @param vm JavaVM pointer.
     * @param obj Android activity object.
     * @param asset_mgr_obj The asset manager object.
     */
    MazeGame(JavaVM *vm, jobject obj, jobject asset_mgr_obj);

    ~MazeGame();

    /**
     * Initializes any GL-related objects. This should be called on the rendering
     * thread with a valid GL context.
     *
     * @param env The JNI environment.
     */
    void OnSurfaceCreated(JNIEnv *env);

    /**
     * Sets screen parameters.
     *
     * @param width Screen width
     * @param height Screen height
     */
    void SetScreenParams(int width, int height);

    /**
     * Draws the scene. This should be called on the rendering thread.
     */
    void OnDrawFrame();

    /**
     * Hides the target object if it's being targeted.
     */
    void OnTriggerEvent();

    /**
     * Pauses head tracking.
     */
    void OnPause();

    /**
     * Resumes head tracking.
     */
    void OnResume();

    /**
     * Allows user to switch viewer.
     */
    void SwitchViewer();

    void UpdateTouching(uint8_t touching) { touching_ = touching; }


    inline static AAssetManager *getAssetManager() { return asset_mgr_; }

    static MazeGame *getInstance();

private:

    static MazeGame *sInstance;

    /**
   * Default near clip plane z-axis coordinate.
   */
    static constexpr float kZNear = 0.01f;

    /**
     * Default far clip plane z-axis coordinate.
     */
    static constexpr float kZFar = 100.f;

    /**
     * Updates device parameters, if necessary.
     *
     * @return true if device parameters were successfully updated.
     */
    bool UpdateDeviceParams();

    /**
     * Initializes GL environment.
     */
    void GlSetup();

    /**
     * Deletes GL environment.
     */
    void GlTeardown();

    static AAssetManager *asset_mgr_;

    CardboardHeadTracker *head_tracker_;
public:
    CardboardHeadTracker *getHeadTracker() const;

private:
    CardboardLensDistortion *lens_distortion_;
    CardboardDistortionRenderer *distortion_renderer_;

    CardboardEyeTextureDescription left_eye_texture_description_;
    CardboardEyeTextureDescription right_eye_texture_description_;

    bool screen_params_changed_;
    bool device_params_changed_;
    int screen_width_;
    bool touching_;

    int screen_height_;

    float projection_matrices_[2][16];
    float eye_matrices_[2][16];

    GLuint depthRenderBuffer_;  // depth buffer
    GLuint framebuffer_;        // framebuffer object
    GLuint texture_;            // distortion texture

    GLuint obj_modelview_projection_param_;

    Matrix4x4 head_view_;


};


#endif  // HELLO_CARDBOARD_ANDROID_SRC_MAIN_JNI_HELLO_CARDBOARD_APP_H_
