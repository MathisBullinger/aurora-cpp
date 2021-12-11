#version 330 core

layout (location = 0) in vec3 vPos;

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(vPos, 1);
}
