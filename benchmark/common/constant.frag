#version 120

uniform float time;

void main( void )
{
  float n = 0.0;
  gl_FragColor = vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
}
