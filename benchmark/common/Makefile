OPTIONS=
SRCDIR=../../src
COMMON=commonShader.frag
SHADERS=simplexnoise2D.frag simplexnoise3D.frag simplexnoise4D.frag \
 classicnoise2D.frag classicnoise3D.frag classicnoise4D.frag

all: $(SHADERS)

clean:
	 - rm $(SHADERS)

simplexnoise2D.frag: $(SRCDIR)/noise2D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"noise2D.glsl\" \
		-DVTYPE=vec2 -DVNAME=v_texCoord2D -DNOISEFUN=snoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

simplexnoise3D.frag: $(SRCDIR)/noise3D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"noise3D.glsl\" \
		-DVTYPE=vec3 -DVNAME=v_texCoord3D -DNOISEFUN=snoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

simplexnoise4D.frag: $(SRCDIR)/noise4D.glsl $(COMMON)
	cpp -P  -I$(SRCDIR) -DSHADER=\"noise4D.glsl\" \
		-DVTYPE=vec4 -DVNAME=v_texCoord4D -DNOISEFUN=snoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

classicnoise2D.frag: $(SRCDIR)/classicnoise2D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"classicnoise2D.glsl\" \
		-DVTYPE=vec2 -DVNAME=v_texCoord2D -DNOISEFUN=cnoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

classicnoise3D.frag: $(SRCDIR)/classicnoise3D.glsl $(COMMON)
	cpp -P -I$(SRCDIR) -DSHADER=\"classicnoise3D.glsl\" \
		-DVTYPE=vec3 -DVNAME=v_texCoord3D -DNOISEFUN=cnoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@

classicnoise4D.frag: $(SRCDIR)/classicnoise4D.glsl $(COMMON)
	cpp -P  -I$(SRCDIR) -DSHADER=\"classicnoise4D.glsl\" \
		-DVTYPE=vec4 -DVNAME=v_texCoord4D -DNOISEFUN=cnoise\
		$(OPTIONS) -DVERSION='#version 120' commonShader.frag $@
