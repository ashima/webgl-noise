//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110403 (stegu)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//

vec3 permute(vec3 x)
{
  return floor(mod(((x*34.0)+1.0)*x, 289.0));
}
  
vec3 taylorInvSqrt(vec3 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec2 v)
  {
  const vec2 C = vec2(0.211324865405187134, // (3.0-sqrt(3.0))/6.0;
                      0.366025403784438597); // 0.5*(sqrt(3.0)-1.0);
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  i1.x = step( x0.y, x0.x ); // 1.0 if x0.x > x0.y, else 0.0
  i1.y = 1.0 - i1.x;

  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;

  vec4 xC = x0.xyxy + vec4( C.xx, -1.0 + 2.0 * C.xx);
  xC.xy -= i1;

// Permutations
  i = mod(i, 289.0); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(xC.xy,xC.xy), dot(xC.zw,xC.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// ( N points uniformly over a line, mapped onto a diamond.)
  vec3 x = 2.0 * fract(p / 41.0) - 1.0 ;
  vec3 h = abs(x) - 0.5 ;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
  m *= taylorInvSqrt( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * xC.xz + h.yz * xC.yw;
  return 130.0 * dot(m, g);
}
