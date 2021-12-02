#version 330 core

in vec2 UV;
in vec3 posWorldSpace;
in vec3 normalCamSpace;
in vec3 eyeDirCamSpace;
in vec3 lightDirCamSpace;

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

const vec3 lightColor = vec3(1);
const float lightPower = 30;
const vec3 materialSpecularColor = vec3(.3);

void main() {
  vec3 ambient = lightColor * material.ambient;

  vec3 lightDir = normalize(lightDirCamSpace);
  float diff = max(dot(normalize(normalCamSpace), lightDir), 0);
  vec3 diffuse = lightColor * diff * material.diffuse;

  vec3 reflectDir = reflect(-lightDir, normalCamSpace);
  float spec = pow(max(dot(normalize(eyeDirCamSpace), reflectDir), 0), material.shine);
  vec3 specular = lightColor * spec * material.specular;

  color = ambient + diffuse + specular;
}
