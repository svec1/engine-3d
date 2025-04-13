#version 460

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vClr;

out vec3 VFragColor;

uniform mat4 M;
uniform mat4 VP;

uniform float gravityConstant;
uniform vec4 objectComponents[1024];

void main(){
    vec4 tmpPos = M * vec4(vPos, 1.0);
    
    for(int i = 0; i < 1024; ++i){
        float distance = (pow(objectComponents[i].x-tmpPos.x, 2)+pow(objectComponents[i].z-tmpPos.z, 2))/pow(objectComponents[i].w*4, 2) + 0.6;
	    tmpPos.y -= (objectComponents[i].y*gravityConstant)/distance;
    }
    gl_Position = VP * tmpPos;
    
    VFragColor = vClr;    
}
