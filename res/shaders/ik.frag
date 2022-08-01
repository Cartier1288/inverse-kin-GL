#version 330 core

uniform sampler2D texture2d;

in float timef;

out vec4 color;

void main() {
    color = vec4(0.0,0.0,0.0,1.0);
}
