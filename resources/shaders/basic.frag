#version 330 core

in vec2 UV;
in vec3 posWorldSpace;
in vec3 normalCamSpace;
in vec3 eyeDirCamSpace;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 lightPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shine;
};
uniform Material material;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

void main() {
  vec3 lightDir = normalize(light.pos + eyeDirCamSpace);
  
  vec3 ambient = light.ambient * material.ambient;

  float diff = max(dot(normalize(normalCamSpace), lightDir), 0);
  vec3 diffuse = light.diffuse * diff * material.diffuse;

  vec3 reflectDir = reflect(-lightDir, normalCamSpace);
  float spec = pow(max(dot(normalize(eyeDirCamSpace), reflectDir), 0), material.shine);
  vec3 specular = light.specular * spec * material.specular;

  color = ambient + diffuse + specular;
}
