#version 410 core

out vec4 FragColor;

uniform float uTime;
//uniform vec4 uColor;

void main() {
    float time = uTime;
    FragColor = vec4(sin(time), 0.5f, 0.2f, 1.0f);
}
