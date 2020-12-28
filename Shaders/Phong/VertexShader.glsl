#version 460

layout(location = 0)in vec3 in_Position;
layout(location = 1)in vec3 in_Normal;
layout(location = 2)in vec2 in_TexCoords;
layout(location = 3)in vec3 in_Tangent;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;

uniform vec3 u_cameraPosition;

out vec2 o_texCoord;
out vec3 o_worldPos;
out vec3 o_normal;
out vec3 o_cameraPosition;
out vec3 o_tangent;

void main(void){
  gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_Position, 1.0);
  
  o_worldPos = vec3(u_modelMat * vec4(in_Position, 1.0));
  o_normal = mat3(transpose(inverse(u_modelMat))) * in_Normal;
  o_cameraPosition = vec3(u_modelMat * vec4(u_cameraPosition, 1.0));
  o_texCoord = in_TexCoords;
  o_tangent = in_Tangent;
}