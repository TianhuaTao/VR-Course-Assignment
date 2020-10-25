//
// Created by tth13 on 9/26/2020.
//

#include "Camera.h"

#include "ContainerBox.h"
#include "MazeGame.h"
#include "util.h"

constexpr uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

Matrix4x4 Camera::GetPose() {
    std::array<float, 4> out_orientation;
    std::array<float, 3> out_position;
    long monotonic_time_nano = GetMonotonicTimeNano();
    monotonic_time_nano += kPredictionTimeWithoutVsyncNanos;
    CardboardHeadTracker_getPose(MazeGame::getInstance()->getHeadTracker(), monotonic_time_nano,
                                 &out_position[0], &out_orientation[0]);
    float x, y, z, w;
    x = out_orientation[0];
    y = out_orientation[1];
    z = out_orientation[2];
    w = out_orientation[3];

    Front.x = 2 * (x * z - w * y);
    Front.y = 2 * (y * z + w * x);
    Front.z = 1 - 2 * (x * x + y * y);

//    return GetTranslationMatrix(out_position) * Quatf::FromXYZW(&out_orientation[0]).ToMatrix();
    return  Quatf::FromXYZW(&out_orientation[0]).ToMatrix();
}