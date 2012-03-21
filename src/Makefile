.PHONY: all clean

PERLIN=classicnoise2D.glsl classicnoise3D.glsl classicnoise4D.glsl 

PRODUCTS=noise.glo

all: ${PRODUCTS}

noise.glo: ${PERLIN} noise.glsl
	gloc -c --accuracy preprocess --dissolve noise.glsl ${PERLIN} -o noise.glo

clean:
	rm -f ${PRODUCTS}