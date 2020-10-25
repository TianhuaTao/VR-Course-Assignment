//
// Created by tth13 on 9/26/2020.
//

#ifndef MAZEGAME_CAMERA_H
#define MAZEGAME_CAMERA_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "util.h"
#include <GLES2/gl2.h>

const float SPEED = 2.5f;

const float ZOOM = 45.0f;

class Camera {
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;

    // camera options
    float MovementSpeed;

    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : MovementSpeed(SPEED), Zoom(ZOOM) {
        Position = position;
    }


    Matrix4x4 GetPose();

};

#endif //MAZEGAME_CAMERA_H
