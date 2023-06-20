#version 410 core

layout (location = 0) in vec3 aPos;

uniform float uTime;
uniform float uHorizontal;

out vec4 vPos;

void main()
{
    vPos = vec4(aPos.x, -1.0f * aPos.y, aPos.z, 1.0);
    gl_Position = vPos;
}
