#version 460
layout(location = 0)in vec3 in_Position;
layout(location = 1)in vec3 in_Normal;
layout(location = 2)in vec2 in_TexCoords;
layout(location = 3)in vec3 in_Tangent;

uniform mat4 u_depthMVP;

void main(void)
{
  gl_Position = u_depthMVP * vec4(in_Position, 1);
}