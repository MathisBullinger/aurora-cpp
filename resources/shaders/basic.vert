#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 posWorldSpace;
out vec3 normalCamSpace;
out vec3 eyeDirCamSpace;

uniform mat4 MVP;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;

void main() {
  gl_Position = MVP * vec4(vertexPosition, 1);
  UV = vertexUV;

  posWorldSpace = (model * vec4(vertexPosition, 1)).xyz;
  vec3 posCam = (view * vec4(posWorldSpace, 1)).xyz;
  eyeDirCamSpace = -posCam;
  normalCamSpace = normal * vertexNormal;
}
