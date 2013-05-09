*webgl-noise* contain Simplex Noise functions that are fast and 
self contained, with performance that scales well with massive
parallelism.
While not quite as fast as Gustavson's original implementation
on typical current desktop GPUs, they are more scalable to
massive parallelism and much more convenient to use, and
they can make good use of unused ALU resources when run
concurrently with a typical texture-intensive rendering.
Gustavson's implementation makes heavy use of a texture
lookup table and is texture bandwidth limited.
 
This work follows Stefan Gustavson's paper "Simplex noise demystified"
http://www.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
without using any uniform arrays or texture engines.

Simplex noise functions are (C) Ashima Arts
Classic noise functions are (C) Stefan Gustavson

###License
[MIT Expat](http://ashimagroup.net/os/license/mit-expat)

###Tags
[webgl](http://ashimagroup.net/os/tag/webgl)

