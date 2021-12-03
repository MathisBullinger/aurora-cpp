#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec3 color;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float specExp;
};
uniform Material material;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;
uniform vec3 viewPos;

void main() {
  vec3 ambient = light.ambient * material.ambient;

  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(light.pos - fragPos);
  float diff = max(dot(normal, lightDir), 0);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specExp);
  vec3 specular = light.specular * (spec * material.specular);

  color = ambient + diffuse + specular;
}
