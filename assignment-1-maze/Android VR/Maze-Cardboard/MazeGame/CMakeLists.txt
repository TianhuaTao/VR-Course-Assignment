# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.4.1)

# C++ flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Standard Android dependencies
#find_library(android-lib android)
#find_library(GLESv3-lib GLESv3)
find_library(log-lib log)

set(libs_dir ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

# === Cardboard Sample ===
# Sources
# Output binary
add_library(# Sets the name of the library.
        cardboard_jni

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/jni/ContainerBox.cpp
        src/main/jni/ContainerBox.h
        src/main/jni/mazeGameInterface.cpp
        src/main/jni/MazeGame.cpp
        src/main/jni/MazeGame.h
        src/main/jni/Shader.h
        src/main/jni/glm
        src/main/jni/stbi/stb_image.cpp
        src/main/jni/stbi/stb_image.h
        src/main/jni/LoadShaders.h
        src/main/jni/LoadShaders.cpp
        src/main/jni/LoadTexture.h
        src/main/jni/LoadTexture.cpp
        src/main/jni/camera.h
        src/main/jni/camera.cpp
        src/main/jni/util.h
        src/main/jni/util.cpp
        )
# Includes
target_include_directories(cardboard_jni PRIVATE ${libs_dir})
# Build
target_link_libraries(cardboard_jni
        EGL
        GLESv2
        android
    ${log-lib}
    ${libs_dir}/jni/${ANDROID_ABI}/libcardboard_api.so)
