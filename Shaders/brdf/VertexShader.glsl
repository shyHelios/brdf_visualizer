#version 460

layout(location = 0)in vec3 in_Position;
layout(location = 1)in vec3 in_Normal;
layout(location = 2)in vec2 in_TexCoords;
layout(location = 3)in vec3 in_Tangent;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
out vec2 o_texCoord;
out vec3 o_objectColor;

uniform vec3 u_incidentVector;

uniform int u_phongShininess;

vec3 phongBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 reflectVector = reflect(toLight, normal);
  float specVal = pow(max(dot(toCamera, reflectVector), 0.0), u_phongShininess);
  return vec3(specVal);
}

vec3 BRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  return phongBRDF(toLight, toCamera, normal, tangent, bitangent);
}

void main(void){
  vec3 normal = vec3(0, 0, 1);
  vec3 tangent = vec3(1, 0, 0);
  vec3 bitangent = vec3(0, 1, 0);
  vec3 lumimance = vec3(0.2126, 0.7152, 0.0722);
  
  vec3 normPos = normalize(in_Position);
  vec3 normIncident = -normalize(u_incidentVector);
  
  vec3 brdf = BRDF(normIncident, normPos, normal, tangent, bitangent);
  float offVal = (lumimance.x * brdf.x) + (lumimance.y * brdf.y)+ (lumimance.z * brdf.z);
  //  float offVal = brdf.x;
  vec3 newPos = normPos * offVal;
  
  gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(newPos, 1.0);
  o_texCoord = in_TexCoords;
  o_objectColor = vec3(offVal);
}