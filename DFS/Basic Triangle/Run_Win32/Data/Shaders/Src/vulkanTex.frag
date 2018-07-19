#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "inc/debug.glsl"

layout(location = 0) in vec4 passColor;
layout(location = 1) in vec2 passUV;

layout(set = 1, binding = 0) uniform sampler2D gTexDiffuse;

layout(location = 0) out vec4 outColor;


void main() 
{
	outColor = texture(gTexDiffuse, passUV);
}