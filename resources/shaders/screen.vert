#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 2) in vec2 vUV;

out vec2 uv;

void main() {
  gl_Position = vec4(vPos.x, vPos.y, 0.0, 1.0);
  uv = vUV;
}
