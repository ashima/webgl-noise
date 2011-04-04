SRCDIR=../../src
MAIN=noisedemoMain.frag
SHADERS=noisedemo.frag

all: $(SHADERS)

clean:
	 - rm $(SHADERS)

noisedemo.frag: $(SRCDIR)/noise3D.glsl $(MAIN)
	cpp -P -I$(SRCDIR) -DSHADER=\"noise3D.glsl\" \
	-DVERSION='#version 120' noisedemoMain.frag $@
