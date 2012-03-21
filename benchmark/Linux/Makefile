SHADERS=noisebench.vert simplexnoise2D.frag simplexnoise3D.frag\
	simplexnoise4D.frag classicnoise2D.frag classicnoise3D.frag\
	classicnoise4D.frag constant.frag
COMDIR=../common
VPATH=$(COMDIR)
EXECNAME=noisebench
OUTPUTFILE=ashimanoise.log

all: $(EXECNAME) links_done

$(EXECNAME): noisebench.c
	gcc -I. -I/usr/X11/include $^ -lglfw -lGLU -lGL -o $@

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

clean:
	- rm $(EXECNAME) $(SHADERS) links_done $(OUTPUTFILE)

run:
	./$(EXECNAME)
	cat $(OUTPUTFILE)
