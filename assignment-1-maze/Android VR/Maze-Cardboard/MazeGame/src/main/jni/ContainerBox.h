//
// Created by tth13 on 9/26/2020.
//

#ifndef MAZEGAME_CONTAINERBOX_H
#define MAZEGAME_CONTAINERBOX_H

#include "MazeGame.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "LoadShaders.h"
#include "LoadTexture.h"
#include "Camera.h"
#include <GLES2/gl2ext.h>
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>

class ContainerBox {
public:

    const static int maze_x_limit = 10;
    const static int maze_y_limit = 1;
    const static int maze_z_limit = 10;
    static int maze_xz[maze_z_limit][maze_x_limit];

    static bool occupied(float x, float z) {
        if (x < -0.58f || x > maze_x_limit - 1 + 0.58f || z < -0.58f || z > maze_x_limit - 1 + 0.58f) {
            return false;
        }
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                float xf = x + i * 0.08f;
                float zf = z + j * 0.08f;

                int xi = (int) (xf + 0.5);
                int zi = (int) (zf + 0.5);
                if (xi < 0 || xi >= maze_x_limit || zi < 0 || zi >= maze_z_limit)
                    continue;
                if (maze_xz[xi][zi])
                    return true;
            }
        }
        return false;

    }

    static void Initialize() {


        genPositions();
        ShaderInfo render_shaders[] =
                {
                        {GL_VERTEX_SHADER,   "shaders/containerBox.vert"},
                        {GL_FRAGMENT_SHADER, "shaders/containerBox.frag"},
                        {GL_NONE, NULL}
                };
        render_prog = LoadShaders(render_shaders);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        // first, configure the cube's VAO (and VBO)

//        glVertexArray(1, &containerVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//        glBindVertexArray(containerVAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // load textures (we now use a utility function to keep the code more organized)
        // -----------------------------------------------------------------------------
        diffuseMap = loadTexture("textures/container.png");
        specularMap = loadTexture("textures/container_specular.png");

    }

    static void draw(glm::mat4 eye_view, glm::mat4 projection) {
        Shader containerShader(render_prog);
        containerShader.use();
        containerShader.setVec3("viewPos", camera.Position);
        containerShader.setFloat("shininess", 32.0f);


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


        containerShader.setMat4("projection", projection);
        containerShader.setMat4("view", eye_view);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // render containers
        for (unsigned int i = 0; i < positions.size(); i++) {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positions[i]);
            containerShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

    }

private:
    static unsigned int VBO;
    static GLuint render_prog;
    static std::vector<glm::vec3> positions;
    static unsigned int diffuseMap;
    static unsigned int specularMap;

    static void genPositions() {


        for (int i = 0; i < maze_z_limit; i++) {
            for (int j = 0; j < maze_x_limit; j++) {
                if (maze_xz[i][j]) {
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

#endif //MAZEGAME_CONTAINERBOX_H
