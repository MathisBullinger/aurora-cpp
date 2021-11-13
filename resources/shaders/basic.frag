#version 330 core

in vec2 UV;
in vec3 posWorldSpace;
in vec3 normalCamSpace;
in vec3 eyeDirCamSpace;
in vec3 lightDirCamSpace;

out vec3 color;

uniform sampler2D textureSampler;

uniform vec3 lightPos;

const vec3 lightColor = vec3(1);
const float lightPower = 15;
const vec3 materialSpecularColor = vec3(.3);

void main() {
  vec3 materialDiffuseColor = texture(textureSampler, UV).rgb;
  vec3 materialAmbientColor = 0.25 * materialDiffuseColor;

  float lightDist = 5; // length(lightPos - posWorldSpace);

  vec3 n = normalize(normalCamSpace);
  vec3 l = normalize(lightDirCamSpace);
  float cosTheta = clamp(dot(n, l), 0, 1);

  vec3 e = normalize(eyeDirCamSpace);
  vec3 r = reflect(-l, n);
  float cosAlpha = clamp(dot(e, r), 0, 1);

  color = 
    materialAmbientColor +
    materialDiffuseColor * lightColor * lightPower * cosTheta / (lightDist*lightDist) +
    materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / (lightDist*lightDist);
}
