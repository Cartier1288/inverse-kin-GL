#version 330 core

uniform sampler2D texture2d;
uniform vec4 JointColor;

in float timef;
in float instanceN;

out vec4 color;

void main() {
  if(JointColor == vec4(0.0,0.0,0.0,1.0)) {
    color = vec4(0.2,abs(sin(instanceN/1.5))/2.0,0.2,1.0);
  }
  else {
    color = JointColor;
  }
}
