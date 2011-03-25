OPTIONS=-DNORMALISE_GRADIENTS -DCOLLAPSE_SORTNET -DNOCLAMP -DNOSTEP
SRCDIR=../../src
COMMON=commonShader.frag $(SRCDIR)/noiseStdLib.glsl
SHADERS=GLSL-ashimanoise2D.frag GLSL-ashimanoise3D.frag \
	GLSL-ashimanoise4D.frag

all: $(SHADERS)

clean:
	 - rm $(SHADERS)

GLSL-ashimanoise2D.frag: $(SRCDIR)/noise2D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"noise2D.glsl\" \
		-DVTYPE=vec2 -DVNAME=v_texCoord2D\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

GLSL-ashimanoise3D.frag: $(SRCDIR)/noise3D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"noise3D.glsl\" \
		-DVTYPE=vec3 -DVNAME=v_texCoord3D\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

GLSL-ashimanoise4D.frag: $(SRCDIR)/noise4D.glsl $(COMMON)
	cpp -P  -I$(SRCDIR) -DSHADER=\"noise4D.glsl\" \
		-DVTYPE=vec4 -DVNAME=v_texCoord4D\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@
