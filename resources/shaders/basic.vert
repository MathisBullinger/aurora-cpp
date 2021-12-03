#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 VP;
uniform mat3 normal;

void main() {
  fragPos = vec3(model * vec4(vertexPosition, 1));
  fragNormal = normal * vertexNormal;
  gl_Position = VP * vec4(fragPos, 1);
}
