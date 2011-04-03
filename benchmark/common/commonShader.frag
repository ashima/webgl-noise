VERSION

#include SHADER

uniform float time; // Used for texture animation

varying VTYPE VNAME ;

//
// main()
//
void main( void )
{
  float n = NOISEFUN(VNAME);
  gl_FragColor = vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
}
