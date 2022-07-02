#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoordIn;
uniform float time;

out float timef;
out vec2 texCoord;

void main() {
    gl_Position = vec4(pos * sin(time), 0.0, 1.0);
    timef = time;
    texCoord = texCoordIn;
}