#version 460
out vec4 fragColor;

in vec3 o_worldPos;
in vec3 o_normal;
in vec2 o_texCoord;
in vec3 o_cameraPosition;
in vec3 o_tangent;

#define MAX_LIGHTS 8

#define M_PI       3.14159265358979323846// pi
#define M_PI_2     1.57079632679489661923// pi/2
#define M_PI_4     0.785398163397448309616// pi/4
#define M_1_PI     0.318309886183790671538// 1/pi
#define M_2_PI     0.636619772367581343076// 2/pi

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  float constant;
  float linear;
  float quadratic;
};

struct Environment{
  sampler2D environmentMap;
  sampler2D GGXIntegrMap;
  sampler2D irradianceMap;
  int maxMip;
};

struct Material {
  vec3 diffuseColor;
  sampler2D diffuseTexture;
  float hasDiffuseTexture;
  
  vec3 specularColor;
  sampler2D specularTexture;
  float hasSpecularTexture;
  
  sampler2D normalTexture;
  float hasNormalTexture;
  
  float roughness;
  sampler2D roughnessTexture;
  float hasRoughnessTexture;
  
  float metalness;
  sampler2D metalnessTexture;
  float hasMetalnessTexture;
  
  float shininess;
  float ior;
};

struct FSMaterial{
  vec4 albedo;
  vec4 specular;
  vec3 normalVector;
  float roughness;
  float metalness;
  float shininess;
  float ior;
};

uniform int u_lightCount;
uniform Material u_material;
uniform Environment u_environment;
uniform Light u_light[MAX_LIGHTS];

FSMaterial fsMaterial;


// ////////////
// Common code
// ///////////

vec4 getAlbedo(){
  return (1.0 - u_material.hasDiffuseTexture) * vec4(u_material.diffuseColor, 1) + u_material.hasDiffuseTexture * texture(u_material.diffuseTexture, o_texCoord);
}

vec4 getSpecular(){
  return (1.0 - u_material.hasSpecularTexture) * vec4(u_material.specularColor, 1) + u_material.hasSpecularTexture * texture(u_material.specularTexture, o_texCoord);
}

float getRoughness(){
  return (1.0 - u_material.hasRoughnessTexture) * u_material.roughness + u_material.hasRoughnessTexture * texture(u_material.roughnessTexture, o_texCoord).x;
}

float getMetalness(){
  return (1.0 - u_material.hasMetalnessTexture) * u_material.metalness + u_material.hasMetalnessTexture * texture(u_material.metalnessTexture, o_texCoord).x;
}

vec4 toneMap(vec4 hdrIn, float exposure, float gamma){
  // Exposure tone mapping
  vec4 mapped = vec4(1.0) - exp(-hdrIn * exposure);
  // Gamma correction
  mapped = pow(mapped, vec4(1.0 / gamma));
  return mapped;
}

vec2 sphericalToCube(vec3 pos){
  float theta = acos(clamp(pos.z, -1, 1));
  float phi = atan(pos.y, pos.x) + M_PI;
  
  float u = 1.f - phi * 0.5f * M_1_PI;
  float v = theta * M_1_PI;
  
  return vec2(u, v);
}

vec3 getNormalFromMap(vec3 normalVector){
  vec3 bumpMapNormal = (texture(u_material.normalTexture, o_texCoord).xyz * 2.0) - vec3(1.0);
  
  vec3 deltaPosX = dFdx(o_worldPos);
  vec3 deltaPosY = dFdy(o_worldPos);
  vec2 deltaTexX = dFdx(o_texCoord);
  vec2 deltaTexY = dFdy(o_texCoord);
  
  vec3 tangent = normalize(deltaPosX * deltaTexY.t - deltaPosY * deltaTexX.t);
  vec3 normal = normalize(normalVector);
  vec3 bitangent = -normalize(cross(normal, tangent));
  mat3 TBN = mat3(tangent, bitangent, normal);
  
  return normalize(TBN * bumpMapNormal);
}

// ////////////
// PBR Specific
// ///////////

float fresnelSchlick(float cosTheta, float F0){
  return F0 + (1 - F0) * pow((1 - cosTheta), 5);
}

float  TrowbridgeReitzGGXDistribution(vec3 normalVector, vec3 halfVector, float roughness){
  //  float roughness2 = roughness * roughness;
  float roughness2 = max(roughness * roughness, 2.0e-3);
  float roughness4 = roughness2 * roughness2;
  
  float normalDotHalf = max(dot(normalVector, halfVector), 0.0);
  float normalDotHalf2 = normalDotHalf * normalDotHalf;
  
  float d = (normalDotHalf2 * (roughness4 - 1.0) + 1.0);
  d = M_PI * d * d;
  
  return roughness4 / d;
}

// Physicaly based shader development for unity 2017, page 165
float GeometrySchlickGGX(float normalDotVector, float k){
  return normalDotVector / (normalDotVector * (1.0 - k) + k);
}

// Physicaly based shader development for unity 2017, page 165
float GeometrySmith(vec3 normalVector, vec3 viewVector, vec3 lightVector, float roughness){
  float normalDotView = max(dot(normalVector, viewVector), 0.0);
  float normalDotLight = max(dot(normalVector, lightVector), 0.0);
  
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;
  
  float g1v = GeometrySchlickGGX(normalDotView, k);
  float g1l = GeometrySchlickGGX(normalDotLight, k);
  
  return g1l * g1v;
}

vec4 cookTorranceBRDF(){
  vec3 eye = o_cameraPosition;
  // View vector, from fragment to camera
  vec3 viewVector = normalize(o_cameraPosition - o_worldPos);
  // Reflect vector, from fragment do reflect direction over normal
  vec3 reflectVector = normalize(reflect(-viewVector, fsMaterial.normalVector));
  
  // Light vector, from fragment to light
  vec3 lightVector = normalize(u_light[0].position - o_worldPos);
  
  // Half vector, between light vector and view vector
  vec3 halfLightViewVector = normalize(lightVector + viewVector);
  
  float normalDotView = max(dot(fsMaterial.normalVector, viewVector), 0.0);
  
  float normalDotLight = max(dot(fsMaterial.normalVector, lightVector), 0.0);
  
  float halfDotView = max(dot(halfLightViewVector, viewVector), 0.0);
  vec3 F0 = vec3(pow((1 - fsMaterial.ior) / (1 + fsMaterial.ior), 2.0));
  vec3 F  = F0 + (1.0 - F0) * pow(1.0 - halfDotView, 5.0);
  
  float NDF = TrowbridgeReitzGGXDistribution(fsMaterial.normalVector, halfLightViewVector, fsMaterial.roughness);
  float G = GeometrySmith(fsMaterial.normalVector, viewVector, lightVector, fsMaterial.roughness);
  
  vec3 specular = (NDF * G * F) / max((4.0 * normalDotView * normalDotLight), 0.001);
  
  vec3 kS = F;
  vec3 kD = vec3(1.0) - kS * (1.0 - fsMaterial.metalness);
  
  vec3 irradiance = texture(u_environment.irradianceMap, sphericalToCube(fsMaterial.normalVector)).rgb;
  
  float mipLevel = u_environment.maxMip * (1.0 - fsMaterial.roughness);
  vec3 env = textureLod(u_environment.environmentMap, sphericalToCube(reflectVector), mipLevel).rgb;
  
  vec2 brdf = texture(u_environment.GGXIntegrMap, vec2(normalDotView, fsMaterial.roughness)).rg;
  
  vec3 Lo = kD * ((fsMaterial.albedo.rgb / M_PI + specular) * irradiance) + (kS * brdf.x + brdf.y) * env;
  //TODO: Lo *= shadow;
  
  return vec4(Lo, 1.0);
}

void main () {
  // Normal vector, computed from surface normal and normal map
  fsMaterial.albedo = getAlbedo();
  fsMaterial.specular = getSpecular();
  fsMaterial.normalVector = normalize(o_normal);
  if (!gl_FrontFacing){
    fsMaterial.normalVector *= -1.;
  }
  
  if (u_material.hasNormalTexture > 0.0){
    fsMaterial.normalVector = getNormalFromMap(fsMaterial.normalVector);
  }
  
  fsMaterial.roughness = getRoughness();
  fsMaterial.roughness = clamp(fsMaterial.roughness, 0.001f, 1.f);
  
  fsMaterial.metalness = getMetalness();
  fsMaterial.shininess = u_material.shininess;
  fsMaterial.ior = u_material.ior;
  
  fragColor = cookTorranceBRDF();
}