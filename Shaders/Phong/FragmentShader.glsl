#version 460
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

vec3 getNormalFromMap(vec3 normalVector)
{
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

vec3 computeLight(Light light){
  //attenuation
  float distance = length(light.position - o_worldPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  
  // ambient
  vec3 ambient = light.ambient;
  
  // diffuse
  vec3 norm = fsMaterial.normalVector;
  vec3 lightDir = normalize(light.position - o_worldPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff;
  
  diffuse *= fsMaterial.albedo.rgb;
  ambient *= fsMaterial.albedo.rgb;
  
  // specular
  vec3 viewDir = normalize(o_cameraPosition - o_worldPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
  vec3 specular = light.specular * spec;
  
  //result
  return (ambient + diffuse + specular) * attenuation;
}

void main()
{
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
  fsMaterial.metalness = getMetalness();
  fsMaterial.shininess = u_material.shininess;
  fsMaterial.ior = u_material.ior;
  
  vec3 res = vec3(0.f);
  
  for (int i = 0; i < u_lightCount; i++){
    res += computeLight(u_light[i]);
  }
  
  fragColor = vec4(res, 1.0);
}