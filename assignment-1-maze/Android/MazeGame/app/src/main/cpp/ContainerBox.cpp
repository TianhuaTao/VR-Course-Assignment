//
// Created by tth13 on 9/26/2020.
//

#include "ContainerBox.h"
#include <vector>
unsigned int ContainerBox::VBO;
unsigned int ContainerBox::containerVAO;
GLuint ContainerBox::render_prog;

std::vector<glm::vec3> ContainerBox::positions;

// positions of the point lights
glm::vec3 pointLightPositions[] = {
        glm::vec3(-1.0f,  2.0f,  -1.0f),
        glm::vec3(-1.0f, 2.0f, 10.0f),
        glm::vec3(10.0f,  2.0f, -1.0f),
        glm::vec3(10.0f,  2.0f, 10.0f)
};

unsigned int  ContainerBox::diffuseMap;
unsigned int  ContainerBox::specularMap;

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