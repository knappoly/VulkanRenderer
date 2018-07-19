#version 420 core

// Uniforms ==============================================
// Constants
uniform vec3 EYE_POSITION;  // camera related

// Scene related
uniform vec4  AMBIENT; // xyz color, w intensity

// lighting
uniform vec3 LIGHT_POSITION; 
uniform vec4 LIGHT_COLOR; // xyz color, w intensity

// surface 
uniform float SPECULAR_AMOUNT; // shininess (0 to 1)
uniform float SPECULAR_POWER; // smoothness (1 to whatever)

// Textures
// Suggest always manually setting bindings - again, consitancy with 
// other rendering APIs and well as you can make assumptions in your
// engine without having to query
layout(binding = 0) uniform sampler2D gTexDiffuse;

// Attributes ============================================
in vec2 passUV; 
in vec4 passColor; 
in vec3 passWorldPos;   // new
in vec3 passWorldNormal;// new

out vec4 outColor; 

// Entry Point ===========================================
void main( void )
{
   // Texture color sampling
   vec4 texColor = texture( gTexDiffuse, passUV );

   // Interpolation is linear, so normals become not normal
   // over a surface, so renormalize it. 
   vec3 surface_normal = normalize(passWorldNormal);

   // Color of this surface
   vec4 surface_color = texture( gTexDiffuse, passUV ) * passColor; 

   // used in final lighting equation to compute
   // final color of output - calculated from the light
   vec3 surface_light = vec3(0); // How much light is hitting the surface
   vec3 reflected_light = vec3(0);  // How much light is reflected back

   // First, some vectors we'll use
   vec3 light_dir = normalize(LIGHT_POSITION - passWorldPos); // direction to light
   vec3 eye_dir = normalize(EYE_POSITION - passWorldPos); // direction to the eye

   // first, add in Ambient light (general brightness of the room)
   // to our surface light
   #if defined(USE_AMBIENT)
   surface_light = AMBIENT.xyz * AMBIENT.w;
   #endif

   // Next, for this light, how much of it is hitting the surface
   #if defined(DOT3)
   float dot3 = dot(light_dir, surface_normal); 
   vec3 light_color = max(dot3, 0.0f) * LIGHT_COLOR.xyz; 
   surface_light += light_color; 
   #endif

   // Next compute the reflected light
   float spec_factor = 0.0f; 

   // get the direction of the light reflected off the surface...
   #if defined(PHONG)
   vec3 r = reflect( -light_dir, surface_normal ); 
   // ... and compute how much it hits our eye.
   spec_factor = max( 0, dot( eye_dir, r ) ); 
   #endif
   
   // Scale and sharpen the reflection to simulate general
   // shininess and roughness of of the surface
   spec_factor = SPECULAR_AMOUNT * pow( spec_factor, SPECULAR_POWER ); 
   
   // Add in the reflected power of this light.
   reflected_light += LIGHT_COLOR.xyz * spec_factor; 

   // Surface lighting should never blow 
   // out your surface.  It is either fully lit or not, 
   // so clamp it.
   surface_light = clamp(surface_light, vec3(0), vec3(1)); 

   // Reflected light on the other hand CAN exceed one (used for bloom)

   // Calculate final color; note, it should not change the alpha... so...
   // Alpha of one for surface light as it is multiplied in,
   // and 0 for specular since it is added in.
   vec4 final_color = vec4(surface_light, 1) * texColor + vec4(reflected_light, 0); 

   // Reclamp so that we're in a valid colour range.  May want to save off overflow
   // if doing bloom.
   final_color = clamp(final_color, vec4(0), vec4(1) ); 
   
   // Output the colour
   outColor = final_color;
}