#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoordIn;

uniform float time;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 offset[3];

out float timef;
out vec2 texCoord;

void main() {
    gl_Position = projection * view * model * vec4(pos+offset[gl_InstanceID], 1.0);
    timef = time;
    texCoord = texCoordIn;
}
