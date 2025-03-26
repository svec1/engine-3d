#version 460

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vClr;

out vec3 VFragColor;

uniform mat4 VP;
uniform mat4 M;

void main()
{
    VFragColor = vClr;
    gl_Position = VP * M * vec4(vPos, 1.0);
} 
