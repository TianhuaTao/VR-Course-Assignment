#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <string>
#include "LoadShaders.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "game.h"
unsigned int loadTexture(const char *path);

/**
 * Everything is this class is static
 * because all boxes will be drawn at once
 * not one by a time
 */
class ContainerBox
{
public:
    /* 10 x 10 maze, height is 1*/
    const static int maze_x_limit = 10;
    const static int maze_y_limit = 1;
    const static int maze_z_limit = 10;

    static int maze_xz[maze_z_limit][maze_x_limit]; // bitmap of maze

    /* return if a point is occupied by a box*/
    static bool occupied(float x, float z)
    {
        // A box is 1.0 wide
        // Set a margin of 0.04

        if (x < -0.54f || x > maze_x_limit - 1 + 0.54f || z < -0.54f || z > maze_x_limit - 1 + 0.54f)
        {
            return false;
        }

        // check 9 points surrounding the camera position
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                float xf = x + i * 0.04f;
                float zf = z + j * 0.04f;

                int xi = (int)(xf + 0.5);
                int zi = (int)(zf + 0.5);
                if (xi < 0 || xi >= maze_x_limit || zi < 0 || zi >= maze_z_limit)
                    continue;
                if (maze_xz[xi][zi])
                    return true;
            }
        }
        return false;
    }
    static void Initialize()
    {
        genPositions();
        ShaderInfo render_shaders[] =
            {
                {GL_VERTEX_SHADER, "shaders/containerBox.vert"},
                {GL_FRAGMENT_SHADER, "shaders/containerBox.frag"},
                {GL_NONE, NULL}};
        render_prog = LoadShaders(render_shaders);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };

        // first, configure the cube's VAO (and VBO)

        glGenVertexArrays(1, &containerVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(containerVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // load textures (we now use a utility function to keep the code more organized)
        diffuseMap = loadTexture("textures/container.png");
        specularMap = loadTexture("textures/container_specular.png");
    }
    static void draw()
    {
        Shader containerShader(render_prog);
        containerShader.use();
        containerShader.setVec3("viewPos", camera.Position);
        containerShader.setFloat("material.shininess", 32.0f); // this is buggy, will fix in lab 2

        // directional light
        containerShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        containerShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        containerShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        containerShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        containerShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        containerShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        containerShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        containerShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        containerShader.setFloat("pointLights[0].constant", 1.0f);
        containerShader.setFloat("pointLights[0].linear", 0.09);
        containerShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        containerShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        containerShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        containerShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        containerShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        containerShader.setFloat("pointLights[1].constant", 1.0f);
        containerShader.setFloat("pointLights[1].linear", 0.09);
        containerShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        containerShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        containerShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        containerShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        containerShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        containerShader.setFloat("pointLights[2].constant", 1.0f);
        containerShader.setFloat("pointLights[2].linear", 0.09);
        containerShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        containerShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        containerShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        containerShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        containerShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        containerShader.setFloat("pointLights[3].constant", 1.0f);
        containerShader.setFloat("pointLights[3].linear", 0.09);
        containerShader.setFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        containerShader.setVec3("spotLight.position", camera.Position);
        containerShader.setVec3("spotLight.direction", camera.Front);
        containerShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        containerShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        containerShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        containerShader.setFloat("spotLight.constant", 1.0f);
        containerShader.setFloat("spotLight.linear", 0.09);
        containerShader.setFloat("spotLight.quadratic", 0.032);
        containerShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        containerShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window_width / (float)window_height, 0.01f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        containerShader.setMat4("projection", projection);
        containerShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        containerShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render containers
        glBindVertexArray(containerVAO);
        for (unsigned int i = 0; i < positions.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);

            containerShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

private:
    static unsigned int VBO, containerVAO;
    static GLuint render_prog;
    static std::vector<glm::vec3> positions;
    static unsigned int diffuseMap;
    static unsigned int specularMap;

    static void genPositions()
    {
        for (int i = 0; i < maze_z_limit; i++)
        {
            for (int j = 0; j < maze_x_limit; j++)
            {
                if (maze_xz[i][j])
                {
                    int xx = 1 * i;
                    int yy = 0;
                    int zz = 1 * j;
                    positions.push_back(glm::vec3(xx, yy, zz));
                }

                // floor
                positions.push_back(glm::vec3(i, -1, j));
            }
        }
    }
};
