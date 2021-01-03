#version 460
out vec4 fragColor;

in vec3 o_objectColor;
in vec2 o_texCoord;


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

void main () {
  //  fragColor = getAlbedo();
  fragColor = vec4(o_objectColor, 1);
}
