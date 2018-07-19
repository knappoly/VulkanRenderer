#version 450
#extension GL_ARB_separate_shader_objects : enable

out gl_PerVertex {
	vec4 gl_Position;
};

layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec4 COLOR;

layout(location = 0) out vec4 passColor;


void main() 
{
	gl_Position = vec4(POSITION, 1.0f);
	passColor = COLOR;
}