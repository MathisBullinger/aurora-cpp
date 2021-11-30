#version 330 core

out vec4 color;

in vec3 normal;
in vec3 position;

uniform vec3 eyePos;
uniform samplerCube skybox;

void main()
{             
  vec3 eyeDir = normalize(position - eyePos);
  vec3 refDir = reflect(eyeDir, normalize(normal));
  refDir.y *= -1;
  color = vec4(texture(skybox, refDir).rgb, 1);
}
