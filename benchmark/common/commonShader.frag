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
  gl_FragColor = vec4(vec3(n * 0.5 + 0.5), 1.0);
}
