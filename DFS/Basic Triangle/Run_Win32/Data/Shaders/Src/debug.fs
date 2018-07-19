#version 420 core								
layout(binding = 0) uniform sampler2D gTexDiffuse;	
in vec2 passUV;										
in vec4 passColor;		
in vec4 passCurColor;							
							
out vec4 outColor;

void main(void)									
{													
	vec4 diffuse = texture( gTexDiffuse, passUV );		
	outColor = diffuse * passColor * passCurColor;				
}