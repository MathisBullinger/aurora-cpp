#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 VP;
uniform mat3 normalMat;

void main() {
  normal = normalMat * vertexNormal;
  position = vec3(model * vec4(vertexPosition, 1));
  gl_Position = VP * vec4(position, 1);
}  
