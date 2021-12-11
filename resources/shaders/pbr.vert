#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

out FRAG {
  vec3 pos;
  vec3 normal;
  vec2 UV;
} frag;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  frag.UV = vUV;
  frag.pos = vec3(model * vec4(vPos, 1.0));
  frag.normal = mat3(model) * vNormal;
  gl_Position = projection * view * vec4(frag.pos, 1.0);
}
