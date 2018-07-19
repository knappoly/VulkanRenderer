// Debug Functions
vec3 WorldNormalToColor( vec3 normal )
{
   return (normal + vec3(1.0f)) * .5f; 
}

vec3 ColorToSurfaceNormal( vec3 color ) 
{
   return normalize( (vec3(2.0f, 2.0f, 1.0f) * color) - vec3( 1.0f, 1.0f, 0.0f ) ); 
}