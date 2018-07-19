#version 450
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 POSITION;
layout(location = 1) in vec4 COLOR;
layout(location = 2) in vec2 UV;

layout(set = 0, binding = 0, std140) uniform CameraBlock
{
	mat4 VIEW;
	mat4 PROJECTION;
};

layout(set = 0, binding = 1, std140) uniform ModelBlock
{
	mat4 MODEL;
};

layout(location = 0) out vec4 passColor;
layout(location = 1) out vec2 passUV;

out gl_PerVertex {
	vec4 gl_Position;
};

void main() 
{
	vec4 localPos = vec4(POSITION, 1.0f);
	vec4 worldPos = MODEL * localPos;
	vec4 cameraPos = VIEW * worldPos;
	vec4 clipPos = PROJECTION * cameraPos;

	gl_Position = clipPos;
	gl_Position.y = -gl_Position.y;
	passColor = COLOR;
	passUV = UV;
}