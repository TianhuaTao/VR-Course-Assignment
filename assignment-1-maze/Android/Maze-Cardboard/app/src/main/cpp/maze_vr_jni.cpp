//
// Created by Sam on 2020/10/23.
//


#include <android/log.h>
#include <jni.h>

#include <memory>

#include "maze_vr_app.h"


#define JNI_METHOD(return_type, method_name) \
  JNIEXPORT return_type JNICALL              \
      Java_com_taotianhua_maze_1vr_MainActivity_##method_name

namespace {

    inline jlong jptr(ndk_maze_vr::MazeVRApp* native_app) {
        return reinterpret_cast<intptr_t>(native_app);
    }

    inline ndk_maze_vr::MazeVRApp* native(jlong ptr) {
        return reinterpret_cast<ndk_maze_vr::MazeVRApp*>(ptr);
    }

    JavaVM* javaVm;

}  // anonymous namespace

extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

JNI_METHOD(jlong, nativeOnCreate)
(JNIEnv* env, jobject obj, jobject asset_mgr) {
    return jptr(new ndk_maze_vr::MazeVRApp(javaVm, obj, asset_mgr));
}

JNI_METHOD(void, nativeOnDestroy)
(JNIEnv* env, jobject obj, jlong native_app) { delete native(native_app); }

JNI_METHOD(void, nativeOnSurfaceCreated)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->OnSurfaceCreated(env);
}

JNI_METHOD(void, nativeOnDrawFrame)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->OnDrawFrame();
}

JNI_METHOD(void, nativeOnTriggerEvent)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->OnTriggerEvent();
}

JNI_METHOD(void, nativeOnPause)
(JNIEnv* env, jobject obj, jlong native_app) { native(native_app)->OnPause(); }

JNI_METHOD(void, nativeOnResume)
(JNIEnv* env, jobject obj, jlong native_app) { native(native_app)->OnResume(); }

JNI_METHOD(void, nativeSetScreenParams)
(JNIEnv* env, jobject obj, jlong native_app, jint width, jint height) {
    native(native_app)->SetScreenParams(width, height);
}

JNI_METHOD(void, nativeSwitchViewer)
(JNIEnv* env, jobject obj, jlong native_app) {
    native(native_app)->SwitchViewer();
}

}  // extern "C"
