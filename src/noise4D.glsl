//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110223
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//              Distributed under the Artistic License 2.0; See LICENCE file.
//

vec4 grad4(float j, vec4 ip)
  {
  const vec4 ones = vec4(1.0,1.0,1.0,-1.0);
  vec4 p,s;

  p.xyz = floor( fract (vec3(j) * ip.xyz) *pParam.w) * ip.z -1.0;
  p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
  s = vec4(lessThan(p, vec4(0.0)));
  p.xyz = p.xyz + (s.xyz*2.0 - 1.0) * s.www; 

  return p;
  }

float simplexNoise(vec4 v)
  {
  const vec2  C = vec2( 0.138196601125010504,  // (5 - sqrt(5))/20  G4
                        0.309016994374947451); // (sqrt(5) - 1)/4   F4
// First corner
  vec4 i  = floor(v + dot(v, C.yyyy) );
  vec4 x0 = v -   i + dot(i, C.xxxx);

// Other corners

#if (0)
  // Rank sorting by Bill Licea-Kane, AMD (formerly ATI).
  // BUG: This works fine om Nvidia hardware, but ironically,
  // on ATI-AMD hardware the sorting is messed up. Why?
  vec4 i0;

  vec3 isX = step( x0.yzw, x0.xxx );
  i0.x = dot( isX, vec3( 1.0 ) );
  i0.yzw = 1.0 - isX;

  vec3 isYZ = step( x0.zww, x0.yyz );
  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
  i0.zw += 1.0 - isYZ.xy;

  i0.z += isYZ.z;
  i0.w += 1.0 - isYZ.z;

  // i0 now contains the unique values 0,1,2,3 in each channel
  vec4 i3 = clamp( i0, 0.0, 1.0 );
  vec4 i2 = clamp( i0-1.0, 0.0, 1.0 );
  vec4 i1 = clamp( i0-2.0, 0.0, 1.0 );
#else
// Force existance of strict total ordering in sort.
  vec4 q0 = floor(x0 * 1024.0) + vec4( 0.0, 1.0/4.0, 2.0/4.0 , 3.0/4.0);
  vec4 q1;
  q1.xy = max(q0.xy, q0.zw);   //  x:z  y:w
  q1.zw = min(q0.xy, q0.zw);

  vec4 q2;
  q2.xz = max(q1.xz, q1.yw);   //  x:y  z:w
  q2.yw = min(q1.xz, q1.yw);
  
  vec4 q3;
  q3.y = max(q2.y, q2.z);      //  y:z
  q3.z = min(q2.y, q2.z);
  q3.xw = q2.xw;

  vec4 i1 = vec4(lessThanEqual(q3.xxxx, q0));
  vec4 i2 = vec4(lessThanEqual(q3.yyyy, q0));
  vec4 i3 = vec4(lessThanEqual(q3.zzzz, q0));
#endif

  //  x0 = x0 - 0.0 + 0.0 * C 
  vec4 x1 = x0 - i1 + 1.0 * C.xxxx;
  vec4 x2 = x0 - i2 + 2.0 * C.xxxx;
  vec4 x3 = x0 - i3 + 3.0 * C.xxxx;
  vec4 x4 = x0 - 1.0 + 4.0 * C.xxxx;

// Permutations
  i = mod(i, pParam.x); 
  float j0 = permute( permute( permute( permute (
              i.w, pParam.xyz) + i.z, pParam.xyz) 
            + i.y, pParam.xyz) + i.x, pParam.xyz);
  vec4 j1 = permute( permute( permute( permute (
             i.w + vec4(i1.w, i2.w, i3.w, 1.0 ), pParam.xyz)
           + i.z + vec4(i1.z, i2.z, i3.z, 1.0 ), pParam.xyz)
           + i.y + vec4(i1.y, i2.y, i3.y, 1.0 ), pParam.xyz)
           + i.x + vec4(i1.x, i2.x, i3.x, 1.0 ), pParam.xyz);
// Gradients
// ( N*N*N points uniformly over a cube, mapped onto a 4-octohedron.)
  vec4 ip = vec4(pParam.w) ;
  ip.xy *= pParam.w ;
  ip.x  *= pParam.w ;
  ip = vec4(1.0,1.0,1.0,2.0) / ip ;

  vec4 p0 = grad4(j0,   ip);
  vec4 p1 = grad4(j1.x, ip);
  vec4 p2 = grad4(j1.y, ip);
  vec4 p3 = grad4(j1.z, ip);
  vec4 p4 = grad4(j1.w, ip);

#ifdef NORMALISE_GRADIENTS
  p0 *= taylorInvSqrt(dot(p0,p0));
  p1 *= taylorInvSqrt(dot(p1,p1));
  p2 *= taylorInvSqrt(dot(p2,p2));
  p3 *= taylorInvSqrt(dot(p3,p3));
  p4 *= taylorInvSqrt(dot(p4,p4));
#endif

// Mix contributions from the five corners
  vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.0);
  vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.0);
  m0 = m0 * m0;
  m1 = m1 * m1;
  return 70.0 * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
               + dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) ) ;

  }


