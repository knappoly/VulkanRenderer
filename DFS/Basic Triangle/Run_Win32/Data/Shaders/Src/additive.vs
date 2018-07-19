#version 420 core

// Attributes
in vec3 POSITION;
in vec4 COLOR;
in vec2 UV; 

out vec2 passUV; 
out vec4 passColor; 

uniform mat4 MODEL;
uniform mat4 VIEW;
uniform mat4 PROJECTION;


void main( void )
{
   vec4 local_pos = vec4( POSITION, 1.0f );	

   vec4 world_pos = MODEL * local_pos; // assume local is world for now; 
   vec4 camera_pos = VIEW * world_pos; 
   vec4 clip_pos =  PROJECTION * camera_pos; 

   passUV = UV; 
   passColor = COLOR * COLOR.w; // pre-multiply the alpha in 
	gl_Position = clip_pos; // we pass out a clip coordinate
}
