#version 330 core

uniform sampler2D texture2d;
uniform vec4 JointColor;

in float timef;

out vec4 color;

void main() {
    color = JointColor;
}
