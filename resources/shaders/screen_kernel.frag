#version 330 core

out vec3 color;
in vec2 uv;

uniform sampler2D tex;
uniform float kernel[9];
uniform float offset;

vec2 step = vec2(offset) / textureSize(tex, 0);

void main() {
  for (int i = 0; i < 9; i++)
    color += vec3(texture(tex, uv.st + vec2(((i % 3) - 1) * step.x, (floor(i / 3) - 1) * step.y))) * kernel[i];
}
