//
// Created by tth13 on 9/26/2020.
//

#ifndef MAZEGAME_LOADSHADERS_H
#define MAZEGAME_LOADSHADERS_H


#include <GLES2/gl2.h>
#include <GLES2/gl2platform.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo*);

//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif //MAZEGAME_LOADSHADERS_H
