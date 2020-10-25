
#include "MazeGame.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <array>
#include <cmath>
#include <fstream>

#include "ContainerBox.h"


MazeGame::MazeGame(JavaVM *vm, jobject obj, jobject asset_mgr_obj)
        : head_tracker_(nullptr),
          lens_distortion_(nullptr),
          distortion_renderer_(nullptr),
          screen_params_changed_(false),
          device_params_changed_(false),
          screen_width_(0),
          screen_height_(0),
          depthRenderBuffer_(0),
          framebuffer_(0),
          texture_(0),

          obj_modelview_projection_param_(0) {
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);

    asset_mgr_ = AAssetManager_fromJava(env, asset_mgr_obj);

    Cardboard_initializeAndroid(vm, obj);
    head_tracker_ = CardboardHeadTracker_create();

    sInstance = this;
}

MazeGame::~MazeGame() {
    CardboardHeadTracker_destroy(head_tracker_);
    CardboardLensDistortion_destroy(lens_distortion_);
    CardboardDistortionRenderer_destroy(distortion_renderer_);
}

void MazeGame::OnSurfaceCreated(JNIEnv *env) {
    ContainerBox::Initialize();
    CHECKGLERROR("OnSurfaceCreated");
}

void MazeGame::SetScreenParams(int width, int height) {
    screen_width_ = width;
    screen_height_ = height;
    screen_params_changed_ = true;
}

void MazeGame::OnDrawFrame() {
    if (!UpdateDeviceParams()) {
        return;
    }

    if (touching_) {
        auto front_no_y = camera.Front;
        front_no_y.y = 0.0f;
        auto tmp_pos = camera.Position - 0.01f * front_no_y;
        if (!ContainerBox::occupied(tmp_pos.x, tmp_pos.z))
            camera.Position = tmp_pos;
    }

    // Update Head Pose.

    head_view_ = camera.GetPose();

    head_view_ =
            head_view_ * GetTranslationMatrix({-camera.Position.x, -camera.Position.y, -camera.Position.z});


    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.49f, 0.73f, 0.92f, 1.0f);    // blue

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Draw eyes views
    for (int eye = 0; eye < 2; ++eye) {
        glViewport(eye == kLeft ? 0 : screen_width_ / 2, 0, screen_width_ / 2,
                   screen_height_);


        Matrix4x4 eye_matrix = GetMatrixFromGlArray(eye_matrices_[eye]);
        Matrix4x4 eye_view = eye_matrix * head_view_;

        glm::mat4 gl_eye_view;
        memcpy(&gl_eye_view, eye_view.m, 16 * sizeof(float));
        glm::mat4 gl_proj;
        memcpy(&gl_proj, projection_matrices_[eye], 16 * sizeof(float));

        ContainerBox::draw(gl_eye_view, gl_proj);
    }

    // Render
    CardboardDistortionRenderer_renderEyeToDisplay(
            distortion_renderer_, /* target_display = */ 0, /* x = */ 0, /* y = */ 0,
            screen_width_, screen_height_, &left_eye_texture_description_,
            &right_eye_texture_description_);

    CHECKGLERROR("onDrawFrame");
}

void MazeGame::OnTriggerEvent() {

}

void MazeGame::OnPause() { CardboardHeadTracker_pause(head_tracker_); }

void MazeGame::OnResume() {
    CardboardHeadTracker_resume(head_tracker_);

    // Parameters may have changed.
    device_params_changed_ = true;

    // Check for device parameters existence in external storage. If they're
    // missing, we must scan a Cardboard QR code and save the obtained parameters.
    uint8_t *buffer;
    int size;
    CardboardQrCode_getSavedDeviceParams(&buffer, &size);
    if (size == 0) {
        SwitchViewer();
    }
    CardboardQrCode_destroy(buffer);
}

void MazeGame::SwitchViewer() {
    CardboardQrCode_scanQrCodeAndSaveDeviceParams();
}

bool MazeGame::UpdateDeviceParams() {
    // Checks if screen or device parameters changed
    if (!screen_params_changed_ && !device_params_changed_) {
        return true;
    }

    // Get saved device parameters
    uint8_t *buffer;
    int size;
    CardboardQrCode_getSavedDeviceParams(&buffer, &size);

    // If there are no parameters saved yet, returns false.
    if (size == 0) {
        return false;
    }

    CardboardLensDistortion_destroy(lens_distortion_);
    lens_distortion_ = CardboardLensDistortion_create(
            buffer, size, screen_width_, screen_height_);

    CardboardQrCode_destroy(buffer);

    GlSetup();

    CardboardDistortionRenderer_destroy(distortion_renderer_);
    distortion_renderer_ = CardboardOpenGlEs2DistortionRenderer_create();

    CardboardMesh left_mesh;
    CardboardMesh right_mesh;
    CardboardLensDistortion_getDistortionMesh(lens_distortion_, kLeft,
                                              &left_mesh);
    CardboardLensDistortion_getDistortionMesh(lens_distortion_, kRight,
                                              &right_mesh);

    CardboardDistortionRenderer_setMesh(distortion_renderer_, &left_mesh, kLeft);
    CardboardDistortionRenderer_setMesh(distortion_renderer_, &right_mesh,
                                        kRight);

    // Get eye matrices
    CardboardLensDistortion_getEyeFromHeadMatrix(
            lens_distortion_, kLeft, eye_matrices_[0]);
    CardboardLensDistortion_getEyeFromHeadMatrix(
            lens_distortion_, kRight, eye_matrices_[1]);
    CardboardLensDistortion_getProjectionMatrix(
            lens_distortion_, kLeft, kZNear, kZFar, projection_matrices_[0]);
    CardboardLensDistortion_getProjectionMatrix(
            lens_distortion_, kRight, kZNear, kZFar, projection_matrices_[1]);

    screen_params_changed_ = false;
    device_params_changed_ = false;

    CHECKGLERROR("UpdateDeviceParams");

    return true;
}

void MazeGame::GlSetup() {
    LOGD("GL SETUP");

    if (framebuffer_ != 0) {
        GlTeardown();
    }

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width_, screen_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    left_eye_texture_description_.texture = texture_;
    left_eye_texture_description_.left_u = 0;
    left_eye_texture_description_.right_u = 0.5;
    left_eye_texture_description_.top_v = 1;
    left_eye_texture_description_.bottom_v = 0;

    right_eye_texture_description_.texture = texture_;
    right_eye_texture_description_.left_u = 0.5;
    right_eye_texture_description_.right_u = 1;
    right_eye_texture_description_.top_v = 1;
    right_eye_texture_description_.bottom_v = 0;

    // Generate depth buffer to perform depth test.
    glGenRenderbuffers(1, &depthRenderBuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width_,
                          screen_height_);
    CHECKGLERROR("Create Render buffer");

    // Create render target.
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture_, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthRenderBuffer_);

    CHECKGLERROR("GlSetup");
}

void MazeGame::GlTeardown() {
    if (framebuffer_ == 0) {
        return;
    }
    glDeleteRenderbuffers(1, &depthRenderBuffer_);
    depthRenderBuffer_ = 0;
    glDeleteFramebuffers(1, &framebuffer_);
    framebuffer_ = 0;
    glDeleteTextures(1, &texture_);
    texture_ = 0;

    CHECKGLERROR("GlTeardown");
}


AAssetManager *MazeGame::asset_mgr_;
MazeGame *MazeGame::sInstance;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

MazeGame *MazeGame::getInstance() {
    return sInstance;
}

CardboardHeadTracker *MazeGame::getHeadTracker() const {
    return head_tracker_;
}





