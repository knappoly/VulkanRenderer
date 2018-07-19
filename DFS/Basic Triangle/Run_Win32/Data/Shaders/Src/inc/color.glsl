

//------------------------------------------------------------------------
// Adapted from http://en.wikipedia.org/wiki/HSL_color_space.
vec3 RGBtoHSL( vec3 rgb )
{
   float mx = max( rgb.r, max( rgb.g, rgb.b ));
   float mn = min( rgb.r, min( rgb.g, rgb.b ));

   float lum = (mx + mn) / 2.0f;
   float hue = 0.0f;
   float sat = 0.0f;

   if (mx != mn) {
      float dist = mx - mn;
      if (lum > 0.5f) {
         sat = dist / (2.0f - mx - mn); 
      } else {
         sat = dist / (mx + mn);
      } 

      if (mx == rgb.r) {
         hue = ((rgb.g - rgb.b) / dist) + ((rgb.g < rgb.b) ? 6.0f : 0.0f);
      } else if (mx == rgb.g) {
         hue = ((rgb.b - rgb.r) / dist) + 2.0f;
      } else {
         hue = ((rgb.r - rgb.g) / dist) + 4.0f;
      }
      hue /= 6.0f;
   }

   return vec3( hue, sat, lum ); 
}

//------------------------------------------------------------------------
// Adapted from http://en.wikipedia.org/wiki/HSL_color_space.
vec3 HSLtoRGB( vec3 hsl )
{
   float h = hsl.x;
   float s = hsl.y;
   float l = hsl.z;

   vec3 ret; 
   float c = s * (1.0f - abs( 2.0f * l - 1.0f ));
   float m = l - .5f * c;
   float x = c * (1.0f - abs(mod( h * 6.0f, 2.0f ) - 1.0f));

   if (h < (1.0f / 6.0f)) {
      ret = vec3( c, x, 0.0f );
   } else if (h < (2.0f / 6.0f)) {
      ret = vec3( x, c, 0.0f );
   } else if (h < (3.0f / 6.0f)) {
      ret = vec3( 0.0f, c, x );
   } else if (h < (4.0f / 6.0f)) {
      ret = vec3( 0.0f, x, c );
   } else if (h < (5.0f / 6.0f)) {
      ret = vec3( x, 0.0f, c );
   } else {
      ret = vec3( c, 0.0f, x );
   }

   ret = ret + vec3( m, m, m );
   return ret;
}