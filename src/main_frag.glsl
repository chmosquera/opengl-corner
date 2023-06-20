#version 410 core

in vec4 vPos;

out vec4 FragColor;

uniform float uTime;
//uniform vec4 uColor;

void main() {
    float time = uTime;
    FragColor = vPos;
}
