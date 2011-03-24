SHADERS=GLSL-ashimanoise.vert GLSL-ashimanoise2D.frag \
	GLSL-ashimanoise3D.frag GLSL-ashimanoise4D.frag
COMDIR=../common
VPATH=$(COMDIR)
EXECNAME=noisebench
OUTPUTFILE=ashimanoise.log

all: $(EXECNAME) links_done

$(EXECNAME): noisebench.c
	gcc -I. -I/usr/X11/include $^ -lglfw -o $@

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

clean:
	- rm $(EXECNAME) $(SHADERS) links_done $(OUTPUTFILE)

run:
	./$(EXECNAME)
	cat $(OUTPUTFILE)

