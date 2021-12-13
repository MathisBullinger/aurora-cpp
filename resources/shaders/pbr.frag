#version 330 core

const float PI = 3.141592653;
#define MAX_LIGHTS 16

out vec4 fragColor;

in FRAG {
  vec3 pos;
  vec3 normal;
  vec2 UV;
} frag;

struct Sampler {
  sampler2D texture;
  vec3 vertex;
};

struct SamplerFloat {
  sampler2D texture;
  float vertex;
};

uniform Sampler albedo;
uniform float metallic;
uniform SamplerFloat roughness;
uniform SamplerFloat ao;

uniform sampler2D normalMap;
uniform bool useNormalMap;

struct Light {
  vec3 position;
  vec3 color;
  float strength;
};
uniform Light lights[MAX_LIGHTS];
uniform uint lightCount;

in vec3 lightPos[MAX_LIGHTS];
in vec3 viewPos;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
  vec3 albedoCl = texture(albedo.texture, frag.UV).rgb * albedo.vertex;
  vec3 normal = useNormalMap ? texture(normalMap, frag.UV).xyz * 2.0 - 1.0 : frag.normal;
  float rough = texture(roughness.texture, frag.UV).r * roughness.vertex;
  float aof = texture(ao.texture, frag.UV).r * ao.vertex;
  
  vec3 N = normalize(normal);
  vec3 V = normalize(viewPos - frag.pos);

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedoCl, metallic);

  vec3 Lo = vec3(0);

  for (uint i = 0u; i < lightCount; i++) {
    vec3 L = normalize(lightPos[i] - frag.pos);
    vec3 H = normalize(V + L);
    float distance = length(lightPos[i] - frag.pos);
    float attenuation = 1.0 / (1.0 + distance * distance);
    vec3 radiance = lights[i].color * attenuation * lights[i].strength;

    float NDF = distributionGGX(N, H, rough);
    float G = geometrySmith(N, V, L, rough);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 n = NDF * G * F; 
    float d = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = n / d;

    vec3 kD = (vec3(1) - F) * (1.0 - metallic);

    Lo += (kD * albedoCl / PI + specular) * radiance * max(dot(N, L), 0.0);
  }

  vec3 ambient = vec3(0.1) * albedoCl * aof;
  vec3 color = ambient + Lo;

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
