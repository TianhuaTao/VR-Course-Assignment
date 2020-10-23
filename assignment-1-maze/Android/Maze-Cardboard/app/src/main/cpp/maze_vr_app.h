//
// Created by Sam on 2020/10/23.
//

#ifndef MAZE_VR_MAZE_VR_APP_H
#define MAZE_VR_MAZE_VR_APP_H

#include <android/asset_manager.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <GLES2/gl2.h>
#include "cardboard.h"
#include "util.h"
namespace ndk_maze_vr {
    class MazeVRApp {
    public:
        /**
         * Creates a MazeVRApp.
         *
         * @param vm JavaVM pointer.
         * @param obj Android activity object.
         * @param asset_mgr_obj The asset manager object.
         */
        MazeVRApp(JavaVM* vm, jobject obj, jobject asset_mgr_obj);

        ~MazeVRApp();

        /**
         * Initializes any GL-related objects. This should be called on the rendering
         * thread with a valid GL context.
         *
         * @param env The JNI environment.
         */
        void OnSurfaceCreated(JNIEnv* env);

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

    private:
        /**
         * Default near clip plane z-axis coordinate.
         */
        static constexpr float kZNear = 0.1f;

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

        /**
         * Gets head's pose as a 4x4 matrix.
         *
         * @return matrix containing head's pose.
         */
        Matrix4x4 GetPose();

        /**
         * Draws all world-space objects for the given eye.
         */
        void DrawWorld();

        /**
         * Draws the target object.
         */
        void DrawTarget();

        /**
         * Draws the room.
         */
        void DrawRoom();

        /**
         * Finds a new random position for the target object.
         */
        void HideTarget();

        /**
         * Checks if user is pointing or looking at the target object by calculating
         * whether the angle between the user's gaze and the vector pointing towards
         * the object is lower than some threshold.
         *
         * @return true if the user is pointing at the target object.
         */
        bool IsPointingAtTarget();

        jobject java_asset_mgr_;
        AAssetManager* asset_mgr_;

        CardboardHeadTracker* head_tracker_;
        CardboardLensDistortion* lens_distortion_;
        CardboardDistortionRenderer* distortion_renderer_;

        CardboardEyeTextureDescription left_eye_texture_description_;
        CardboardEyeTextureDescription right_eye_texture_description_;

        bool screen_params_changed_;
        bool device_params_changed_;
        int screen_width_;
        int screen_height_;

        float projection_matrices_[2][16];
        float eye_matrices_[2][16];

        GLuint depthRenderBuffer_;  // depth buffer
        GLuint framebuffer_;        // framebuffer object
        GLuint texture_;            // distortion texture

        GLuint obj_program_;
        GLuint obj_position_param_;
        GLuint obj_uv_param_;
        GLuint obj_modelview_projection_param_;

        Matrix4x4 head_view_;
        Matrix4x4 model_target_;

        Matrix4x4 modelview_projection_target_;
        Matrix4x4 modelview_projection_room_;

        TexturedMesh room_;
        Texture room_tex_;

        std::vector<TexturedMesh> target_object_meshes_;
        std::vector<Texture> target_object_not_selected_textures_;
        std::vector<Texture> target_object_selected_textures_;
        int cur_target_object_;
    };

}
#endif //MAZE_VR_MAZE_VR_APP_H
