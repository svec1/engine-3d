#version 460

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vClr;

out vec4 newVPos;
out vec3 VFragColor;

uniform mat4 M;
uniform mat4 VP;
uniform vec4 objectComponents[256];

void main()
{
    vec4 tmpPos = M * vec4(vPos, 1.0);
    for(int i = 0; i < 256; ++i){
        float distance = (pow(objectComponents[i].x-tmpPos.x, 2)+pow(objectComponents[i].z-tmpPos.z, 2))/(objectComponents[i].w*2);
        distance = max(distance, 2);
        tmpPos.y -= (objectComponents[i].y/2)/distance;
    }
    gl_Position = VP * tmpPos;
    
    VFragColor = vClr;    
    newVPos = tmpPos;
}
