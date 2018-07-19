
//--------------------------------------------------------------------------------------
// LIGHTING
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// DEFINES
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
#define MAX_LIGHTS 8

//--------------------------------------------------------------------------------------
// STRUCTURES
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
struct light_t 
{
   vec4 color;    // alpha is intensity

   // data can only cross a 16-byte boundry IF it starts on a 16-byte boundary.
   vec3 position; 
   float padding0; 

   vec3 direction; 
   float direction_factor; 

   vec3 attenuation; 
   float dot_inner_angle; 

   vec3 spec_attenuation; 
   float dot_outer_angle; 
}; 

//--------------------------------------------------------------------------------------
struct light_factor_t 
{
   vec3 diffuse; 
   vec3 specular; 
}; 

//--------------------------------------------------------------------------------------
// CONSTANT BUFFER
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
layout (binding=5, std140) uniform cLightBlock
{
   vec4 AMBIENT; // alpha is intensity
   light_t LIGHTS[MAX_LIGHTS];
};

//--------------------------------------------------------------------------------------
layout (binding=6, std140) uniform cSpecBlock
{
   float SPEC_FACTOR; 
   float SPEC_POWER;
   vec2 padding0;  
}; 

//--------------------------------------------------------------------------------------
// FUNCTIONS
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// i: light intensity
// d: distance from light
// a: attenuation factors;
float GetAttenuation( float i, float d, vec3 a )
{
   return i / (a.x + d * a.y + d * d * a.z);
}

//--------------------------------------------------------------------------------------
light_factor_t CalculateLightFactor( vec3 position, 
   vec3 eye_dir, 
   vec3 normal, 
   light_t light, 
   float spec_factor, 
   float spec_power )
{
   light_factor_t lf; 

   vec3 light_color = light.color.xyz;

   // get my direction to the light, and distance
   vec3 light_dir = light.position - position; // direction TO the light
   float dist = length(light_dir); 
   light_dir /= dist; 

   // 
   vec3 light_forward = normalize(light.direction); 

   // get the power
   float light_power = light.color.w; 

   // figure out how far away angle-wise I am from the forward of the light (useful for spot lights)
   float dot_angle = dot( light_forward, -light_dir ); 

   // falloff for spotlights.
   float angle_attenuation = smoothstep( light.dot_outer_angle, light.dot_inner_angle, dot_angle ); 
   light_power = light_power * angle_attenuation; 

   // get actual direction light is pointing (spotlights point in their "forward", point lights point everywhere (ie, toward the point))
   light_dir = mix(light_dir, -light_forward, light.direction_factor);

   float attenuation = clamp( GetAttenuation( light_power, dist, light.attenuation ), 0, 1 ); 
   float spec_attenuation = GetAttenuation( light_power, dist, light.spec_attenuation );

   // Finally, calculate dot3 lighting
   float dot3 = dot( light_dir, normal ); 
   float diffuse_factor = clamp( attenuation * dot3, 0.0f, 1.0f );

   // specular

   vec3 r = reflect(-light_dir, normal); 
   float spec_amount = max(dot(r, eye_dir), 0.0f); 
   float spec_intensity = (spec_attenuation * spec_factor) * pow(spec_amount, spec_power); 

   lf.diffuse = light_color * diffuse_factor;
   lf.specular = light_color * spec_intensity; 

   return lf; 
}

//--------------------------------------------------------------------------------------
light_factor_t CalculateLighting( vec3 world_pos, 
   vec3 eye_dir, 
   vec3 normal, 
   float spec_factor, 
   float spec_power ) 
{
   light_factor_t lf; 
   lf.diffuse = AMBIENT.xyz * AMBIENT.w; 
   lf.specular = vec3(0.0f); 

   spec_factor *= SPEC_FACTOR; 
   spec_power *= SPEC_POWER; 

   for (uint i = 0; i < MAX_LIGHTS; ++i) {
      light_factor_t l = CalculateLightFactor( world_pos, eye_dir, normal, LIGHTS[i], spec_factor, spec_power ); 
      lf.diffuse += l.diffuse;
      lf.specular += l.specular; 
   }

   lf.diffuse = clamp( lf.diffuse, vec3(0.0f), vec3(1.0f) ); 
   return lf; 
}


