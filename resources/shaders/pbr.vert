#version 330 core

#define MAX_LIGHTS 16

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;

out FRAG {
  vec3 pos;
  vec3 normal;
  vec2 UV;
} frag;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;

uniform bool useNormalMap;

struct Light {
  vec3 position;
  vec3 color;
  float strength;
};
uniform Light lights[MAX_LIGHTS];
uniform uint lightCount;
uniform vec3 camPos;

out vec3 lightPos[MAX_LIGHTS];
out vec3 viewPos;

void main() {
  frag.UV = vUV;
  frag.pos = vec3(model * vec4(vPos, 1));
  gl_Position = projection * view * vec4(frag.pos, 1);

  if (!useNormalMap) {
    viewPos = camPos;
    frag.normal = normal * vNormal;
    for (int i = 0; i < MAX_LIGHTS; i++) 
      lightPos[i] = lights[i].position;
    return;
  }

  mat3 TBN = transpose(mat3(
    normalize(vec3(model * vec4(vTangent, 0))),
    normalize(vec3(model * vec4(vBitangent, 0))),
    normalize(vec3(model * vec4(vNormal, 0)))
  ));

  viewPos = TBN * camPos;
  frag.pos = TBN * frag.pos;
  for (int i = 0; i < MAX_LIGHTS; i++)
    lightPos[i] = TBN * lights[i].position;
}
