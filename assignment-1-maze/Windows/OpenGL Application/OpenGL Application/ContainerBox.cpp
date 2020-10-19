#include "ContainerBox.h"

unsigned int ContainerBox::VBO;
unsigned int ContainerBox::containerVAO;
GLuint ContainerBox::render_prog;

std::vector<glm::vec3> ContainerBox::positions;

// positions of the point lights
// four virtual lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 2.0f, 9.0f),
    glm::vec3(9.0f, 2.0f, 9.0f),
    glm::vec3(9.0f, 2.0f, 9.0f)};

unsigned int ContainerBox::diffuseMap;
unsigned int ContainerBox::specularMap;

int ContainerBox::maze_xz[ContainerBox::maze_z_limit][ContainerBox::maze_x_limit] = {
       {1,1,1,0,1,1,1,1,1,1,},
       {1,0,0,0,0,0,1,1,0,1,},
       {1,0,1,0,1,1,1,0,0,1,},
       {1,1,1,0,1,0,1,0,1,1,},
       {1,0,0,0,1,0,0,0,1,1,},
       {1,0,1,0,0,0,1,0,0,1,},
       {1,0,1,1,1,1,0,0,0,1,},
       {1,1,0,0,0,0,0,1,0,1,},
       {1,0,0,1,0,1,0,0,0,1,},
       {1,0,1,1,1,1,1,1,1,1,},
};