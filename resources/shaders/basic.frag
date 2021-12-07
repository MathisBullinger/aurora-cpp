#version 330 core

in FRAG {
  vec3 position;
  vec3 normal;
  vec2 UV;
} frag;

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

uniform sampler2D normalMap;
uniform bool useNormalMap;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;

in vec3 tanLightPos;
in vec3 tanViewPos;

void main() {
  vec3 matAmb = material.ambient;
  vec3 matDif = material.diffuse;

  if (useTexture) {
    matAmb = texture(tex, frag.UV).rgb;
    matDif = texture(tex, frag.UV).rgb;
  }

  vec3 normal = normalize(useNormalMap
    ? texture(normalMap, frag.UV).xyz * 2 - 1
    : frag.normal
  );
  
  vec3 ambient = light.ambient * matAmb;

  vec3 lightDir = normalize(tanLightPos - frag.position);
  float dif = max(dot(normal, lightDir), 0);
  vec3 diffuse = light.diffuse * (dif * matDif);

  vec3 viewDir = normalize(tanViewPos - frag.position);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specExp);
  vec3 specular = light.specular * (spec * material.specular);

  color = ambient + diffuse + specular;
}
