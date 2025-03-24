#version 460

in vec3 VFragColor;
in vec4 newVPos;
out vec3 FragColor;

void main()
{
    vec4 nColor = normalize(newVPos)/3;
    FragColor = vec3(VFragColor.x + nColor.y, VFragColor.y + nColor.y, VFragColor.z + nColor.y);
}
