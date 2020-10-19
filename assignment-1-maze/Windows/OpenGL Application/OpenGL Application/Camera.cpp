#include "Camera.h"
#include "ContainerBox.h"
// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
// #define CAMERA_Y_FIXED // limit y = 0
#if defined(CAMERA_Y_FIXED)
    glm::vec3 front_no_y = Front;
    front_no_y.y = 0;
    if (front_no_y.x == 0 && front_no_y.z == 0)
    {
        front_no_y = glm::vec3(0);
    }
    else
    {
        front_no_y = glm::normalize(front_no_y);
    }
    glm::vec3 right_no_y = Right;
    right_no_y.y = 0;
    right_no_y = glm::normalize(right_no_y);
    if (right_no_y.x == 0 && right_no_y.z == 0)
    {
        right_no_y = glm::vec3(0);
    }
    else
    {
        right_no_y = glm::normalize(right_no_y);
    }
    glm::vec3 newPos = Position;
    if (direction == FORWARD)
        newPos += front_no_y * velocity;
    if (direction == BACKWARD)
        newPos -= front_no_y * velocity;
    if (direction == LEFT)
        newPos -= right_no_y * velocity;
    if (direction == RIGHT)
        newPos += right_no_y * velocity;

    if (!ContainerBox::occupied(newPos.x, newPos.z))
    {
        Position = newPos;
    }
#else
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
#endif // defined(CAMERA_Y_FIXED)
}
