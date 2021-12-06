#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUV;

out vec3 color;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float specExp;
};
uniform Material material;

uniform sampler2D tex;
uniform bool useTexture;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;
uniform vec3 viewPos;

void main() {
  vec3 matAmb = material.ambient;
  vec3 matDif = material.diffuse;
  if (useTexture) {
    matAmb = texture(tex, fragUV).rgb;
    matDif = texture(tex, fragUV).rgb;
  }
  
  vec3 ambient = light.ambient * matAmb;

  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(light.pos - fragPos);
  float dif = max(dot(normal, lightDir), 0);
  vec3 diffuse = light.diffuse * (dif * matDif);

  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specExp);
  vec3 specular = light.specular * (spec * material.specular);

  color = ambient + diffuse + specular;
}
