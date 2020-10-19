
#version 400 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec3 vColor;

out vec4 oColor;
void main()
{
    gl_Position = vPosition;
    oColor = vec4(vColor.r, vColor.g, vColor.b, 1.0);
}
