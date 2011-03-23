VERSION

// uniform vec4 pParam; 
// Example constant with a 289-element permutation
const vec4 pParam = vec4( 17.0*17.0, 34.0, 1.0, 7.0);

#include "noiseStdLib.glsl"
#include SHADER

uniform float time; // Used for texture animation

varying VTYPE VNAME ;
//
// main()
//
void main( void )
{
  float n = simplexNoise(VNAME);

  gl_FragColor = vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
}
