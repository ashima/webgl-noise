//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110223
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//              Distributed under the Artistic License 2.0; See LICENCE file.
//

#define NORMALIZE_GRADIENTS
#undef USE_CIRCLE
#define COLLAPSE_SORTNET
#define MOREDOTS

float permute(float x0,vec3 p) { 
  float x1 = mod(x0 * p.y, p.x);
  return floor(  mod( (x1 + p.z) *x0, p.x ));
  }
vec2 permute(vec2 x0,vec3 p) { 
  vec2 x1 = mod(x0 * p.y, p.x);
  return floor(  mod( (x1 + p.z) *x0, p.x ));
  }
vec3 permute(vec3 x0,vec3 p) { 
  vec3 x1 = mod(x0 * p.y, p.x);
  return floor(  mod( (x1 + p.z) *x0, p.x ));
  }
vec4 permute(vec4 x0,vec3 p) { 
  vec4 x1 = mod(x0 * p.y, p.x);
  return floor(  mod( (x1 + p.z) *x0, p.x ));
  }

uniform vec4 pParam; 
// Example
// const vec4 pParam = vec4( 17.* 17., 34., 1., 7.);

float taylorInvSqrt(float r)
  {
  return ( 0.83666002653408 + 0.7*0.85373472095314 - 0.85373472095314 * r );
  }

float simplexNoise2(vec2 v)
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
  vec3 h = 0.5 - abs(x) ;

  vec3 sx = 2.*floor(x) + 1.;
  vec3 sh = floor(h);

  vec3 a0 = x + sx*sh;

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

  return 1.66666* 70. *2. * dot(m, g);
#else 
// N points around a unit circle.
  vec3 phi = D.z * mod(p,pParam.w) /pParam.w ;
  vec4 a0 = sin(phi.xxyy+D.xyxy);
  vec2 a1 = sin(phi.zz  +D.xy);
// mix
  vec3 g = vec3( dot(a0.xy, x0), dot(a0.zw, xC.xy), dot(a1.xy, xC.zw) );
  return 1.66666* 70.*dot(m, g);
#endif
  }

float simplexNoise3(vec3 v)
  { 
  const vec2  C = vec2(1./6. , 1./3. ) ;
  const vec4  D = vec4(0., 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;
  
// Other corners
#ifdef COLLAPSE_SORTNET
  vec3 g = vec3( greaterThan(   x0.xyz, x0.yzx) );
//  vec3 l = vec3( lessThanEqual( x0.xyz, x0.yzx) );
  vec3 l = 1. - g;
  vec3 i1 = g.xyz * l.zxy;
  vec3 i2 = max( g.xyz, l.zxy);
#else
// Keeping this clean - let the compiler optimize.
// Force existance of strict total ordering in sort.
  vec3 q0 = floor(x0 * 1024.0) + vec3( 0., 1./4., 2./4.);
  vec3 q1;
  q1.x = max(q0.x, q0.y);
  q1.y = min(q0.x, q0.y);
  q1.z = q0.z;

  vec3 q2;
  q2.x = max(q1.x,q1.z);
  q2.z = min(q1.x,q1.z);
  q2.y = q1.y;

  vec3 q3;
  q3.y = max(q2.y, q2.z);
  q3.z = min(q2.y, q2.z);
  q3.x = q2.x;

  vec3 i1 = vec3(lessThanEqual(q3.xxx, q0));
  vec3 i2 = vec3(lessThanEqual(q3.yyy, q0));
#endif

   //  x0 = x0 - 0. + 0. * C 
  vec3 x1 = x0 - i1 + 1. * C.xxx;
  vec3 x2 = x0 - i2 + 2. * C.xxx;
  vec3 x3 = x0 - 1. + 3. * C.xxx;

// Permutations
  i = mod(i, pParam.x ); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0., i1.z, i2.z, 1. ), pParam.xyz)
           + i.y + vec4(0., i1.y, i2.y, 1. ), pParam.xyz) 
           + i.x + vec4(0., i1.x, i2.x, 1. ), pParam.xyz);

// Gradients
// ( N*N points uniformly over a square, mapped onto a octohedron.)
  float n_ = 1.0/pParam.w ;
  vec3  ns = n_ * D.wyz - D.xzx ;

  vec4 j = p - pParam.w*pParam.w*floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z)  ;
  vec4 y_ = floor(j - pParam.w * x_ ) ;    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1. - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,D.xxxx)) *2. -1.;
  //vec4 s1 = vec4(lessThan(b1,D.xxxx)) *2. -1.;
  vec4 s0 = floor(b0) *2. +1.;
  vec4 s1 = floor(b1) *2. +1.;
  vec4 sh = -vec4(lessThan(h, D.xxxx));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

#ifdef NORMALISE_GRADIENTS
  p0 *= taylorInvSqrt(dot(p0,p0));
  p1 *= taylorInvSqrt(dot(p1,p1));
  p2 *= taylorInvSqrt(dot(p2,p2));
  p3 *= taylorInvSqrt(dot(p3,p3));
#endif

// Mix
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.);
  m = m * m;
//used to be 64.
  return 48.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
  }

vec4 grad4(float j, vec4 ip)
  {
  const vec4 ones = vec4(1.,1.,1.,-1.);
  vec4 p,s;

  p.xyz = floor( fract (vec3(j) * ip.xyz) *pParam.w) * ip.z -1.0;
  p.w = 1.5 - dot(abs(p.xyz), ones.xyz);
  s = vec4(lessThan(p,vec4(0.)));
  p.xyz = p.xyz + (s.xyz*2.-1.) * s.www; 

  return p;
  }

float simplexNoise4(vec4 v)
  {
  const vec2  C = vec2( 0.138196601125010504,  // (5 - sqrt(5))/20  G4
                        0.309016994374947451); // (sqrt(5) - 1)/4   F4
// First corner
  vec4 i  = floor(v + dot(v, C.yyyy) );
  vec4 x0 = v -   i + dot(i, C.xxxx);

// Other corners

// Force existance of strict total ordering in sort.
  vec4 q0 = floor(x0 * 1024.0) + vec4( 0., 1./4., 2./4. , 3./4.);
  vec4 q1;
  q1.xy = max(q0.xy,q0.zw);   //  x:z  y:w
  q1.zw = min(q0.xy,q0.zw);

  vec4 q2;
  q2.xz = max(q1.xz,q1.yw);   //  x:y  z:w
  q2.yw = min(q1.xz,q1.yw);
  
  vec4 q3;
  q3.y = max(q2.y,q2.z);      //  y:z
  q3.z = min(q2.y,q2.z);
  q3.xw = q2.xw;

  vec4 i1 = vec4(lessThanEqual(q3.xxxx, q0));
  vec4 i2 = vec4(lessThanEqual(q3.yyyy, q0));
  vec4 i3 = vec4(lessThanEqual(q3.zzzz, q0));

   //  x0 = x0 - 0. + 0. * C 
  vec4 x1 = x0 - i1 + 1. * C.xxxx;
  vec4 x2 = x0 - i2 + 2. * C.xxxx;
  vec4 x3 = x0 - i3 + 3. * C.xxxx;
  vec4 x4 = x0 - 1. + 4. * C.xxxx;

// Permutations
  i = mod(i, pParam.x ); 
  float j0 = permute( permute( permute( permute (
              i.w, pParam.xyz) + i.z, pParam.xyz) 
            + i.y, pParam.xyz) + i.x, pParam.xyz);
  vec4 j1 = permute( permute( permute( permute (
             i.w + vec4(i1.w, i2.w, i3.w, 1. ), pParam.xyz)
           + i.z + vec4(i1.z, i2.z, i3.z, 1. ), pParam.xyz)
           + i.y + vec4(i1.y, i2.y, i3.y, 1. ), pParam.xyz)
           + i.x + vec4(i1.x, i2.x, i3.x, 1. ), pParam.xyz);
// Gradients
// ( N*N*N points uniformly over a cube, mapped onto a 4-octohedron.)
  vec4 ip = vec4(pParam.w) ;
  ip.xy *= pParam.w ;
  ip.x  *= pParam.w ;
  ip = vec4(1.,1.,1.,2.) / ip ;

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

// Mix
  vec3 m0 = max(0.6 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.);
  vec2 m1 = max(0.6 - vec2(dot(x3,x3), dot(x4,x4)            ), 0.);
  m0 = m0 * m0;
  m1 = m1 * m1;
  return 32. * ( dot(m0*m0, vec3( dot( p0, x0 ), dot( p1, x1 ), dot( p2, x2 )))
               + dot(m1*m1, vec2( dot( p3, x3 ), dot( p4, x4 ) ) ) ) ;

  }


