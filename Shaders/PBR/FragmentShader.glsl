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

vec3 getIBLIrradiance(vec3 specular, float NdotV){
  vec2 irradiance = texture(u_environment.irradianceMap, vec2(u_material.roughness, 1 - NdotV)).rg;
  return specular * irradiance.r + irradiance.g;
}

float fresnelSchlickRoughness(float cosTheta, float F0, float roughness){
  return F0 + (max((1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float fresnelSchlick(float cosTheta, float F0){
  return F0 + (1 - F0) * pow((1 - cosTheta), 5);
}

float disney(vec3 normalVector, vec3 halfVector, float roughness, Light light){
  vec3 lightVector = normalize(light.position - o_worldPos);
  
  float NdotL = max(dot(normalVector, lightVector), 0.0);
  float LdotH = max(dot(lightVector, halfVector), 0.0);
  float NdotV = max(dot(normalVector, o_cameraPosition), 0.0);
  
  float energyBias = mix(0.0, 0.5, roughness);
  float energyFactor = mix(1.0, 1.0 / 1.51, roughness);
  float fd90 = energyBias + 2.0 * (LdotH * LdotH) * roughness;
  
  float f0 = ((1. - u_material.ior) / (1. + u_material.ior));
  
  float lightScatter = fresnelSchlick(f0, NdotL);
  float viewScatter = fresnelSchlick(f0, NdotV);
  
  return lightScatter * viewScatter * energyFactor;
}

vec3 GGX(vec3 normalVector, vec3 halfVector, float roughness, Light light){
  //  float roughness2 = roughness * roughness;
  float roughness2 = max(roughness * roughness, 2.0e-3);
  float roughness4 = roughness2 * roughness2;
  
  float normalDotHalf = max(dot(normalVector, halfVector), 0.0);
  float normalDotHalf2 = normalDotHalf * normalDotHalf;
  
  float d = (normalDotHalf * roughness4 - normalDotHalf) * normalDotHalf + 1.0;
  float D = roughness4 / (M_PI * (d * d));
  
  vec3 lightVector = normalize(light.position - o_worldPos);
  
  // Fresnel
  vec3 reflectivity = vec3(fsMaterial.specular);
  float fresnel = 1.0;
  float NdotL = clamp(dot(fsMaterial.normalVector, lightVector), 0.0, 1.0);
  float LdotH = clamp(dot(lightVector, halfVector), 0.0, 1.0);
  float NdotV = clamp(dot(fsMaterial.normalVector, o_worldPos), 0.0, 1.0);
  vec3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473 * LdotH - 6.98316) * LdotH);
  
  // geometric / visibility
  float k = roughness2 * 0.5;
  float G_SmithL = NdotL * (1.0 - k) + k;
  float G_SmithV = NdotV * (1.0 - k) + k;
  float G = 0.25 / (G_SmithL * G_SmithV);
  
  return G * D * F;
}

vec3 IBL(){
  float NdotV = max(dot(fsMaterial.normalVector, o_cameraPosition), 0.0);
  
  vec3 reflectionVector = normalize(reflect(-o_cameraPosition, fsMaterial.normalVector));
  
  float mipLevel = 6 * (1.0-fsMaterial.roughness);
  
  vec4 cs = textureLod(u_environment.environmentMap, sphericalToCube(reflectionVector), mipLevel);
  vec3 result = cs.xyz * getIBLIrradiance(fsMaterial.specular.rgb, NdotV);
  
  vec3 diffuseDominantDirection = fsMaterial.normalVector;
  float diffuseLowMip = 6;
  vec3 diffuseImageLighting = textureLod(u_environment.environmentMap, sphericalToCube(diffuseDominantDirection), diffuseLowMip).rgb;
  
  return result + diffuseImageLighting * fsMaterial.albedo.rgb;
}

vec4 cookTorranceBRDF(){
  vec3 eye = o_cameraPosition;
  // View vector, from fragment to camera
  vec3 viewVector = normalize(o_cameraPosition - o_worldPos);
  // Reflect vector, from fragment do reflect direction over normal
  vec3 reflectVector = normalize(reflect(-viewVector, fsMaterial.normalVector));
  
  float normalDotView = max(dot(fsMaterial.normalVector, viewVector), 0.0);
  
  vec3 loDiffuse = vec3(0);
  vec3 loSpecular = vec3(0);
  
  for (int i = 0; i < u_lightCount; i++){
    // Light vector, from fragment to light
    vec3 lightVector = normalize(u_light[i].position - o_worldPos);
    
    // Half vector, between view vector and light vector
    vec3 halfVector = normalize(viewVector + lightVector);
    
    // Compute microfacet normal distribution function
    //float microfacetNormal = microfacetNormalDistributionGGX(normalVector, halfVector, u_material.roughness);
    
    float normalDotLight = max(dot(fsMaterial.normalVector, lightVector), 0.0);
    
    float distance = length(u_light[i].position - o_worldPos);
    float attenuation = 1.0 / (u_light[i].constant + u_light[i].linear * distance + u_light[i].quadratic * (distance * distance));
    
    vec3 diffuse  = normalDotLight * disney(fsMaterial.normalVector, halfVector, fsMaterial.roughness, u_light[i]) * u_light[i].diffuse * attenuation;
    vec3 specular = normalDotLight * GGX(fsMaterial.normalVector, halfVector, fsMaterial.roughness, u_light[i]) * u_light[i].diffuse * attenuation;
    
    loDiffuse += diffuse;
    loSpecular += specular;
  }
  loDiffuse = vec3(0);
  //loSpecular = vec3(0);
  
  float F0 = pow(((1. - u_material.ior) / (1. + u_material.ior)), 2);
  float kS = fresnelSchlick(normalDotView, F0);
  float kD = (1.0 - kS) * (1.0 - fsMaterial.metalness);
  vec2 brdf = texture(u_environment.GGXIntegrMap, vec2(normalDotView, fsMaterial.roughness)).xy;
  
  float mipLevel = u_environment.maxMip * (1.0 - fsMaterial.roughness);
  vec3 LSr = textureLod(u_environment.environmentMap, sphericalToCube(reflectVector), mipLevel).rgb;
  vec3 LDr = (fsMaterial.albedo.rgb) * texture(u_environment.irradianceMap, fsMaterial.normalVector.xy).rgb;
  
  vec3 diffuse = vec3(kD) * LDr + loDiffuse;
  vec3 specular = vec3(kS * brdf.x + brdf.y) * LSr + loSpecular;
  vec3 Lo = diffuse + specular;
  
  return vec4(Lo, 1.0) * fsMaterial.albedo;
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
  //fragColor = getAlbedo();
}
