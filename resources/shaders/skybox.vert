#version 330 core

layout (location = 0) in vec3 pos;

out vec3 texCoords;

uniform mat4 transform;

void main() {
  texCoords = vec3(pos.x, -pos.y, pos.z);
  vec4 pos_ = transform * vec4(pos, 1);
  gl_Position = pos_.xyww;
}
