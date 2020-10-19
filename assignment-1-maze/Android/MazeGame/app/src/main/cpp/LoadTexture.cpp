//
// Created by tth13 on 9/26/2020.
//

#include "LoadTexture.h"
#include "stbi/stb_image.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3platform.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include "MazeGame.h"
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    AAsset *assetFile = AAssetManager_open(MazeGame::getAssetManager(), path, AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(assetFile);
    off_t start = 0, length = 0;
    int fd = AAsset_openFileDescriptor(assetFile, &start, &length);
    FILE * file = fdopen(fd, "rb");
    lseek(fd, start, SEEK_CUR); //NOTICE
    int width, height, nrComponents;
    unsigned char* data = stbi_load_from_file(file, &width, &height, &nrComponents, 0);
    fclose(file);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}