#version 420 core

in vec4 passColor;
// Outputs
out vec4 outColor; 

// Entry Point
void main( void )
{
   outColor = passColor; 
}
