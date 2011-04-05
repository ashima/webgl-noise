VERSION

#include SHADER

uniform float time; // Used for texture animation

varying vec3 v_texCoord3D;

//
// main()
//
void main( void )
{
#if (1)
  // Perturb the texcoords with three components of noise
  vec3 uvw = v_texCoord3D + 0.1*vec3(snoise(v_texCoord3D + vec3(0.0, 0.0, time)),
    snoise(v_texCoord3D + vec3(43.0, 17.0, time)),
	snoise(v_texCoord3D + vec3(-17.0, -43.0, time)));
  // Six components of noise in a fractal sum
  float n = snoise(uvw - vec3(0.0, 0.0, time));
  n += 0.5 * snoise(uvw * 2.0 - vec3(0.0, 0.0, time*1.4)); 
  n += 0.25 * snoise(uvw * 4.0 - vec3(0.0, 0.0, time*2.0)); 
  n += 0.125 * snoise(uvw * 8.0 - vec3(0.0, 0.0, time*2.8)); 
  n += 0.0625 * snoise(uvw * 16.0 - vec3(0.0, 0.0, time*4.0)); 
  n += 0.03125 * snoise(uvw * 32.0 - vec3(0.0, 0.0, time*5.6)); 
  n = n * 0.7;
  // A "hot" colormap - cheesy but effective 
  gl_FragColor = vec4(vec3(1.0, 0.5, 0.0) + vec3(n, n, n), 1.0);
#else
  // A very plain monochrome noise
  float n = snoise(v_texCoord3D * 6.0);
  gl_FragColor = vec4(0.5 + 0.5 * vec3(n, n, n), 1.0);
#endif
}
