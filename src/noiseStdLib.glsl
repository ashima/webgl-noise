//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions. Library function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110227
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//

#ifdef FASTMOD
# define PERMMOD(X,Y) (X)
#else
# define PERMMOD(X,Y) mod((X),(Y))
#endif

#define PERMFUN(X) X permute(X x, vec3 p) { \
  return floor( mod( (PERMMOD(x * p.y, p.x) + p.z)*x, p.x)); }

PERMFUN(float)
PERMFUN(vec2)
PERMFUN(vec3)
PERMFUN(vec4)

#define TAYLOR_L07(X) X taylorInvSqrt(X r) { \
  return ( 1.195228609334394 + 0.7*0.85373472095314 - 0.85373472095314 * r ); }

TAYLOR_L07(float)
TAYLOR_L07(vec2)
TAYLOR_L07(vec3)
TAYLOR_L07(vec4)


