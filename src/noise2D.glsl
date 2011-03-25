//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110325
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//              Distributed under the Artistic License 2.0; See LICENCE file.
//

float simplexNoise(vec2 v)
  {
  const vec2 C = vec2(0.211324865405187134, // (3.0-sqrt(3.0))/6.;
                      0.366025403784438597); // 0.5*(sqrt(3.0)-1.);
  const vec3 D = vec3( 0., 0.5, 2.0) * 3.14159265358979312;
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  i1.x = float( (x0.x>x0.y) );
  i1.y = 1. - i1.x;

  // x0 = x0 - 0. + 0. * C.xx ;
  // x1 = x0 - i1 + 1. * C.xx ;
  // x2 = x0 - 1. + 2. * C.xx ;

  vec4 xC = x0.xyxy + vec4( C.xx, -1. + 2.* C.xx);
  xC.xy -= i1;

// Permutations
  i = mod(i, pParam.x);
  vec3 p = permute( permute( 
             i.y + vec3(0., i1.y, 1. ), pParam.xyz)
           + i.x + vec3(0., i1.x, 1. ), pParam.xyz);

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(xC.xy,xC.xy), dot(xC.zw,xC.zw)), 0.);
  m = m*m ;
  m = m*m ;
#ifndef USE_CIRCLE
// ( N points uniformly over a line, mapped onto a diamond.)
  vec3 x = 2.0 * fract(p / pParam.w) - 1. ;
  vec3 h = abs(x) - 0.5 ;

  vec3 ox = floor(x+0.5);

  vec3 a0 = x - ox;

# ifdef NORMALISE_GRADIENTS
  m *= taylorInvSqrt( a0*a0 + h*h );
# endif
  //vec2 p0 = vec2(a0.x,h.x);
  //vec2 p1 = vec2(a0.y,h.y);
  //vec2 p2 = vec2(a0.z,h.z);
  //vec3 g = vec3( dot(p0, x0), dot(p1, xC.xy), dot(p2, xC.zw) );

  vec3 g;
// a0 *= m;
// h  *= m;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * xC.xz + h.yz * xC.yw;

  return 160.0 * dot(m, g);
#else 
// N points around a unit circle.
  vec3 phi = D.z * mod(p,pParam.w) /pParam.w ;
  vec4 a0 = sin(phi.xxyy+D.xyxy);
  vec2 a1 = sin(phi.zz  +D.xy);
// mix
  vec3 g = vec3( dot(a0.xy, x0), dot(a0.zw, xC.xy), dot(a1.xy, xC.zw) );
  return 160.0 * dot(m, g);
#endif
  }
