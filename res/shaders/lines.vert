#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoordIn;

uniform float time;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 offset[4];

out float timef;
out vec2 texCoord;

out float instanceN;
out vec3 v1;
out vec3 v2;
out mat3 rot;

void main() {
  gl_Position = vec4(pos, 1.0);
  timef = time;
  texCoord = texCoordIn;
  instanceN = gl_InstanceID;

  v1 = (projection * view * model * vec4(offset[gl_InstanceID], 1.0)).xyz;
  v2 = (projection * view * model * vec4(offset[gl_InstanceID + 1], 1.0)).xyz;
  
  // rotate about v1, so that we are working with
  // a simple rectangle.

  float d = v2.x - v1.x;
  float h = length(v2 - v1);

  float theta = acos(abs(d)/h);
  vec2 quad = step(v1.xy,v2.xy);
  if(quad.x+quad.y==1) { theta = -theta; }
  theta = theta + 3.14159265*step(v2.x, v1.x);

  rot = mat3(
    cos(theta), -sin(theta), 0,
    sin(theta),  cos(theta), 0,
             0,           0, 1
  );

  v2 = rot * (v2-v1) + v1;
}
