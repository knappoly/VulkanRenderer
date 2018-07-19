#version 420 core
uniform mat4 PROJECTION;								
uniform mat4 VIEW; 									
uniform mat4 MODEL;

in vec3 POSITION;								
in vec4 COLOR;										
in vec2 UV;											

out vec4 passColor; 									
out vec2 passUV;									

void main( void )								
{											
 	vec4 localPos = vec4(POSITION,1);			
	vec4 clipPos = PROJECTION * VIEW * MODEL * localPos;		
	gl_Position = clipPos;							
	passColor = COLOR;								
	passUV = UV;									
}					