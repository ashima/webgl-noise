SHADERS=GLSL-ashimanoise.vert GLSL-ashimanoise2D.frag \
	GLSL-ashimanoise3D.frag GLSL-ashimanoise4D.frag
COMDIR=../common
VPATH=$(COMDIR)

all: noisebench links_done

noisebench: noisebench.c
	gcc -I. -I/usr/X11/include $^ -lglfw -o $@

links_done: $(SHADERS)
	ln -s $? . ; touch links_done


