#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;

out FRAG {
  vec3 position;
  vec3 normal;
  vec2 UV;
} frag;

uniform mat4 model;
uniform mat4 VP;
uniform mat3 normal;
uniform mat3 TBN;

struct Light {
  vec3 pos;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;
uniform bool useNormalMap;

out vec3 tanLightPos;
out vec3 tanViewPos;

void main() {
  frag.position = vec3(model * vec4(vertexPosition, 1));
  frag.UV = vertexUV;
  gl_Position = VP * vec4(frag.position, 1);

  tanLightPos = light.pos;
  tanViewPos = viewPos;

  if (!useNormalMap) {
    frag.normal = normal * vertexNormal;
    return;
  }

  mat3 TBN = transpose(mat3(
    normalize(vec3(model * vec4(vertexTangent, 0))),
    normalize(vec3(model * vec4(vertexBitangent, 0))),
    normalize(vec3(model * vec4(vertexNormal, 0)))
  ));

  tanLightPos = TBN * light.pos;
  tanViewPos = TBN * viewPos;

  frag.position = TBN * frag.position;
}
