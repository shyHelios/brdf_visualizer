#version 400

layout(location = 0)in vec3 in_Position;
layout(location = 1)in vec3 in_Normal;
layout(location = 2)in vec2 in_TexCoords;
layout(location = 3)in vec3 in_Tangent;

#define M_PI    (3.14159265358979323846264338327950288)
#define M_2PI   (M_PI + M_PI)
#define M_PI2   (M_PI * M_PI)

#define PHONG_BRDF 0
#define BLINN_PHONG_BRDF (PHONG_BRDF + 1)
#define PHONG_PHYS_CORRECT_BRDF (BLINN_PHONG_BRDF + 1)
#define LAMBERT_BRDF (PHONG_PHYS_CORRECT_BRDF + 1)
#define TORRANCE_SPARROW_BRDF (LAMBERT_BRDF + 1)
#define COOK_TORRANCE_BRDF (TORRANCE_SPARROW_BRDF + 1)
#define OREN_NAYAR_BRDF (COOK_TORRANCE_BRDF + 1)
#define MIRROR (OREN_NAYAR_BRDF + 1)

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
out vec2 o_texCoord;
out vec3 o_objectColor;
out vec3 o_normal;

uniform vec3 u_incidentVector;

uniform int u_phongShininess = 16;
uniform float u_phongDiffuse = 0.5;
uniform float u_phongSpecular = 0.5;

uniform float u_roughness = 0.7;
uniform float u_f0 = 0.7;

uniform float u_lambertReflectance = 0.5;

uniform float u_orenNayarReflectance = 0.5;
uniform float u_orenNayarRoughness = 0.1;

uniform int u_brdf;

float phongBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 reflectVector = reflect(-toLight, normal);
  float specVal = pow(max(dot(toCamera, reflectVector), 0.0), u_phongShininess);
  return specVal;
}

float blinnPhongBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 halfVector = normalize(toLight + toCamera);
  float specVal = pow(max(dot(normal, halfVector), 0.0), u_phongShininess);
  //  float specVal = pow(dot(normal, halfVector), u_phongShininess);
  return specVal;
}

float phongPhysicallyCorrectBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 reflectVector = reflect(-toLight, normal);
  float specVal = pow(max(dot(toCamera, reflectVector), 0.0), u_phongShininess);
  //  return (u_phongDiffuse / M_PI) + (((u_phongSpecular * (u_phongShininess + 2)) / M_2PI) * pow(max(dot(toCamera, reflectVector), 0.0), u_phongShininess));
  return (u_phongDiffuse / M_PI) + (((u_phongSpecular * (u_phongShininess + 2)) / M_2PI) * specVal);
}

float lambertBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  return u_lambertReflectance / M_PI;
}

float beckmannDistribution(float roughness, float normDotHalf){
  float roughness2 = roughness*roughness;
  float normDotHalf2 = normDotHalf*normDotHalf;
  float normDotHalf4 = normDotHalf2*normDotHalf2;
  return exp((normDotHalf2-1)/(roughness2*normDotHalf2)) / (roughness2*normDotHalf2);
}

// https://en.wikipedia.org/wiki/Schlick%27s_approximation
float schlick(float r0, float cosTheta){
  return r0 + (1-r0) * pow(1-cosTheta, 5);
}

float geometricAttenuation(vec3 toLight, vec3 toCamera, vec3 normal){
  vec3 halfVector = normalize(toLight + toCamera);
  float normDotHalf = dot(normal, halfVector);
  float toCamDotHalf = dot(toCamera, halfVector);
  float normDotToCamera = dot(normal, toCamera);
  float normDotToLight = dot(normal, toLight);
  
  float res1 = (2*normDotHalf*normDotToCamera)/toCamDotHalf;
  float res2 = (2*normDotHalf*normDotToLight)/toCamDotHalf;
  
  float res = min(1, min(res1, res2));
  return res;
}

float cookTorranceBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 halfVector = normalize(toLight + toCamera);
  
  float normDotHalf = dot(normal, halfVector);
  float toCamDotHalf = dot(toCamera, halfVector);
  
  float normDotToLight = dot(normal, toLight);
  float normDotToCamera = dot(normal, toLight);
  
  float D = beckmannDistribution(u_roughness, normDotHalf);
  float F = schlick(u_f0, toCamDotHalf);
  float G = geometricAttenuation(toLight, toCamera, normal);
  
  // float specVal = D * F * G;
  
  float specVal = (F/M_PI) * (D/normDotToLight) * (G / normDotToCamera);
  
  return specVal;
}

float torranceSparrowBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 halfVector = normalize(toLight + toCamera);
  
  float normDotHalf = dot(normal, halfVector);
  float toCamDotHalf = dot(toCamera, halfVector);
  
  float normDotToLight = dot(normal, toLight);
  float normDotToCamera = dot(normal, toLight);
  
  float D = beckmannDistribution(u_roughness, normDotHalf);
  float F = schlick(u_f0, toCamDotHalf);
  float G = geometricAttenuation(toLight, toCamera, normal);
  
  float specVal = /*kd / M_PI*/ +(/*ks **/ D * F * G) / (4 * M_PI * normDotToLight);
  
  return specVal;
}

float orenNayarBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  float toCamDotNormal = dot(toCamera, normal);
  float toLightDotNormal = dot(toLight, normal);
  float rough2 = u_orenNayarRoughness * u_orenNayarRoughness;
  
  float cosPhiri = dot(normalize(toCamera - normal * toCamDotNormal), normalize(toLight - normal * toLightDotNormal));
  
  float thetaI = acos(toLightDotNormal);
  float thetaO = acos(toCamDotNormal);
  
  float alpha = max (thetaI, thetaO);
  float beta = min (thetaI, thetaO);
  
  float C1 = 1. - 0.5 * (rough2 / (rough2 + 0.33));
  
  float C2;
  if (cosPhiri >= 0){
    C2 = 0.45 * (rough2 / (rough2 + 0.09)) * sin(alpha);
  }
  else {
    C2 =  0.45 * (rough2 / (rough2 + 0.09)) * (sin(alpha) - pow(((2. * beta) / M_PI), 3));
  }
  
  float C3 = 0.125 * (rough2 / (rough2 + 0.09))*((4 * alpha * beta) / M_PI2);
  
  float L1r = (u_orenNayarReflectance / M_PI) * (C1 + cosPhiri*C2*tan(beta) + (1. - abs(cosPhiri)) * C3 * tan((alpha+beta)/2.));
  float L2r = 0.17 * ((u_orenNayarReflectance * u_orenNayarReflectance) / M_PI) * (rough2 / (rough2 + 0.09)) * pow(((4. * alpha * beta)/M_PI2), 2);
  return L1r + L2r;
}

float mirrorBRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  vec3 reflectVector = reflect(-toLight, normal);
  if (reflectVector == toCamera){
    return 1.f;
  }
  return 0.f;
}

float BRDF(vec3 toLight, vec3 toCamera, vec3 normal, vec3 tangent, vec3 bitangent){
  float res;
  switch (u_brdf){
    case PHONG_BRDF:{
      res = phongBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case BLINN_PHONG_BRDF:{
      res = blinnPhongBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case PHONG_PHYS_CORRECT_BRDF:{
      res = phongPhysicallyCorrectBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case LAMBERT_BRDF:{
      res = lambertBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case TORRANCE_SPARROW_BRDF:{
      res = torranceSparrowBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case COOK_TORRANCE_BRDF:{
      res = cookTorranceBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case OREN_NAYAR_BRDF:{
      res = orenNayarBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
    
    case MIRROR:{
      res = mirrorBRDF(toLight, toCamera, normal, tangent, bitangent);
      break;
    }
  }
  
  return res;
}

void main(void){
  const vec3 normal = vec3(0, 0, 1);
  const vec3 tangent = vec3(1, 0, 0);
  const vec3 bitangent = vec3(0, 1, 0);
  const vec3 lumimance = vec3(0.2126, 0.7152, 0.0722);
  
  vec3 normPos = normalize(in_Position);
  vec3 normIncident = normalize(u_incidentVector);
  
  float brdf = BRDF(normIncident, normPos, normal, tangent, bitangent);
  vec3 newPos = normPos * brdf;
  
  gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(newPos, 1.0);
  o_texCoord = in_TexCoords;
  o_objectColor = vec3(brdf);
  o_normal = in_Normal;
}