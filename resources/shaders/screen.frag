#version 330 core

out vec3 color;
in vec2 uv;
uniform sampler2D tex;

void main() {
  color = texture(tex, uv).rgb;
  color = color / (vec3(1) + color); // reinhard tone mapping
  color = pow(color, vec3(1.0/2.2)); // gamma correction
}
