#version 330 core

const float PI = 3.141592653;
#define MAX_LIGHTS 64

out vec4 fragColor;

in FRAG {
  vec3 pos;
  vec3 normal;
  vec2 UV;
} frag;

struct Color {
  sampler2D texture;
  vec3 color;
};

uniform Color albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

struct Light {
  vec3 position;
  vec3 color;
  float strength;
};
uniform Light lights[MAX_LIGHTS];
uniform uint lightCount;

uniform vec3 camPos;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
  vec3 N = normalize(frag.normal);
  vec3 V = normalize(camPos - frag.pos);

  vec3 albedoCl = texture(albedo.texture, frag.UV).rgb * albedo.color;

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedoCl, metallic);

  vec3 Lo = vec3(0);

  for (uint i = 0u; i < lightCount; i++) {
    vec3 L = normalize(lights[i].position - frag.pos);
    vec3 H = normalize(V + L);
    float distance = length(lights[i].position - frag.pos);
    float attenuation = 1.0 / (1.0 + distance * distance);
    vec3 radiance = lights[i].color * attenuation * lights[i].strength;

    float NDF = distributionGGX(N, H, roughness);
    float G = geometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 n = NDF * G * F; 
    float d = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = n / d;

    vec3 kD = (vec3(1) - F) * (1.0 - metallic);

    Lo += (kD * albedoCl / PI + specular) * radiance * max(dot(N, L), 0.0);
  }

  vec3 ambient = vec3(0.03) * albedoCl * ao;
  vec3 color = ambient + Lo;

  // HDR tonemapping
  color = color / (color + vec3(1.0));

  fragColor = vec4(color, 1.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
  float r = roughness * roughness * roughness * roughness;
  float ndh = max(dot(N, H), 0.0);
  float d = ndh * ndh * (r - 1.0) + 1.0;
  return r / (PI * d * d);
}

float geometrySchlickGGX(float NdotV, float roughness) {
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;
  return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = geometrySchlickGGX(NdotV, roughness);
  float ggx1 = geometrySchlickGGX(NdotL, roughness);
  return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
