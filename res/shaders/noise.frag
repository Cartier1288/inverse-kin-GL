#version 330 core

uniform sampler2D texture2d;
uniform vec2 WindowSize;

in float timef;
in vec2 texCoord;

out vec4 color;

float rand(vec2 v) {
  return fract(sin(dot(v, vec2(14.5, 23.9))) * 231.3123);
}

void main() {
    float z = rand(gl_FragCoord.xy);
    float x = gl_FragCoord.x - WindowSize.x/2;
    float y = gl_FragCoord.y - WindowSize.y/2;
    
    z = sin(sqrt(x*x+y*y)/5-timef*40);
 
    color = vec4(z,z,z,0.9);

    z = z * 0.025;
    vec2 disUV = vec2(texCoord.x + z, texCoord.y + z);
    color = texture(texture2d, disUV);
}
