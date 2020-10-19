#pragma once
#define BUFFER_OFFSET(a) ((void *)(a))
class Camera;

const int window_width = 2000;
const int window_height = 1500;
extern Camera camera;
extern glm::vec3 pointLightPositions[];