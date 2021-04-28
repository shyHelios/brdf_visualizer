#version 400
#define MAX_LIGHTS 8
out vec4 fragColor;

in vec3 o_worldPos;
in vec3 o_normal;
in vec2 o_texCoord;
in vec3 o_cameraPosition;
in vec3 o_tangent;


#define M_PI       3.14159265358979323846// pi
#define M_PI_2     1.57079632679489661923// pi/2
#define M_PI_4     0.785398163397448309616// pi/4
#define M_1_PI     0.318309886183790671538// 1/pi
#define M_2_PI     0.636619772367581343076// 2/pi

struct Environment{
  sampler2D environmentMap;
  sampler2D GGXIntegrMap;
  sampler2D irradianceMap;
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

uniform Material u_material;

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

vec3 getNormalFromMap(vec3 normalVector)
{
  // Convert normal vector from <-1,1> to <0,1>
  vec3 bumpMapNormal = (texture(u_material.normalTexture, o_texCoord).xyz * 2.0) - vec3(1.0);
  
  // Get derivations over triangle
  vec3 deltaPosX = dFdx(o_worldPos);
  vec3 deltaPosY = dFdy(o_worldPos);
  
  // Get derivations over texcoord
  vec2 deltaTexX = dFdx(o_texCoord);
  vec2 deltaTexY = dFdy(o_texCoord);
  
  vec3 tangent = normalize(deltaPosX * deltaTexY.t - deltaPosY * deltaTexX.t);
  vec3 normal = normalize(normalVector);
  vec3 bitangent = -normalize(cross(normal, tangent));
  mat3 TBN = mat3(tangent, bitangent, normal);
  
  return normalize(TBN * bumpMapNormal);
}

void main()
{
  fsMaterial.normalVector = normalize(o_normal);
  if (!gl_FrontFacing){
    fsMaterial.normalVector *= -1.;
  }
  
  if (u_material.hasNormalTexture > 0.0){
    fsMaterial.normalVector = getNormalFromMap(fsMaterial.normalVector);
  }
  
  fragColor = vec4((fsMaterial.normalVector.xyz + 1.)/2., 1.0);
}