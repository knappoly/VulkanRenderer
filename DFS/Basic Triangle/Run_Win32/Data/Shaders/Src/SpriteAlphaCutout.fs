#version 420 core

// Input Textures (color and depth)
layout(binding = 0) uniform sampler2D gTexColor;

// Inputs (just UV and Color 
in vec2 passUV; 
in vec4 passColor; 

// Output - a single color; 
out vec4 outColor; 

// Entry Point
void main( void )
{
   vec4 color = texture( gTexColor, passUV ); 
   
   if(color.a == 0)
   {
   	discard;
   }
  
  outColor = color;
}
