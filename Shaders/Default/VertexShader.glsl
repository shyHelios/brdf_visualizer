#version 460

layout(location = 0)in vec3 in_Position;
layout(location = 1)in vec3 in_Normal;
layout(location = 2)in vec2 in_TexCoords;
layout(location = 3)in vec3 in_Tangent;

uniform mat4 u_modelMat;
uniform mat4 u_viewMat;
uniform mat4 u_projMat;
out vec2 o_texCoord;

void main(void){
  gl_Position = u_projMat * u_viewMat * u_modelMat * vec4(in_Position, 1.0);
  
  o_texCoord = in_TexCoords;
}