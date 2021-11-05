#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 MVP;
uniform mat4 model;

void main() 
{
  gl_Position = MVP * vec4(position, 1);

  fragPos = vec3(model * vec4(position, 1));
  normal = vertexNormal;
}
