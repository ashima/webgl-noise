The wiki for this repository contains more information.

Simplex noise functions are (C) Ashima Arts
Classic noise functions are (C) Stefan Gustavson

Source code for the noise functions is released under the
conditions of the MIT license. See the file LICENSE for details.

This work follows Stefan Gustavson's paper "Simplex noise demystified"
http://www.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
without using any uniform arrays or texture engines.

The resulting noise functions are fast and self contained,
and their performance scales well with massive parallelism.
While not quite as fast as Gustavson's original implementation
on typical current desktop GPUs, they are more scalable to
massive parallelism and much more convenient to use, and
they can make good use of unused ALU resources when run
concurrently with a typical texture-intensive rendering.
Gustavson's implementation makes heavy use of a texture
lookup table and is texture bandwidth limited.
