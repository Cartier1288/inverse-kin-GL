#version 330 core

uniform sampler2D texture2d;
uniform vec4 JointColor;
uniform float WIDTH;
uniform float HEIGHT;
uniform float stroke = 0.01;

in float timef;
in float instanceN;

in vec3 v1;
in vec3 v2;
in mat3 rot;

out vec4 color;


// found this efficient solution at: https://stackoverflow.com/a/26697650/2565202
float insideBox(vec2 v, vec2 bottomLeft, vec2 topRight) {
  vec2 s = step(bottomLeft, v) - step(topRight, v);
  return s.x * s.y;
}

void main() {
  vec3 p = rot * (vec3((gl_FragCoord.x / WIDTH) * 2.0 - 1.0, (gl_FragCoord.y / HEIGHT) * 2.0 - 1.0, gl_FragCoord.z) - v1) + v1;

  // after rotation, v1 and v2 have the same y value, so:
  vec2 bottomLeft = vec2(min(v1.x,v2.x), v1.y - stroke);
  vec2 topRight = vec2(max(v1.x,v2.x), v1.y + stroke);

  float N = instanceN + smoothstep(0.0,1.0,abs((p.x-v1.x)/(v2.x-v1.x)));
  color = vec4(0.2,abs(sin(N/2))/2.0,0.2,1.0) * insideBox(p.xy, bottomLeft, topRight);
}
