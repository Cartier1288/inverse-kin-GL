#version 330 core

uniform sampler2D texture2d;

in float timef;
in vec2 texCoord;

out vec4 color;

void main() {
    color = texture(texture2d, texCoord);//vec4(gl_FragCoord.x / 1280, gl_FragCoord.y / 720, 1.0, 1.0);
}