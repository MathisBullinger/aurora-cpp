#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec3 color;

uniform sampler2D textureSampler;

void main() 
{
  vec3 lightColor = vec3(1, 1, 1);
  vec3 lightPos = vec3(-5, 5, -2);
  
  vec3 lightDir = normalize(lightPos - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  color = diffuse;
}
