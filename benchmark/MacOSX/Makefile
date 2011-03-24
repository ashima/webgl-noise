EXECNAME=noisebench
OUTPUTFILE=ashimanoise.log
COMDIR=../common

OBJS=noisebench.o
SHADERS=GLSL-ashimanoise.vert GLSL-ashimanoise2D.frag \
	GLSL-ashimanoise3D.frag GLSL-ashimanoise4D.frag

VPATH=$(COMDIR)
CFLAGS=-I. -I/usr/X11/include
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw

.PHONY: all clean run

all: $(EXECNAME).app links_done

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

$(EXECNAME).app: $(EXECNAME)
	./bundle.sh $@ $^ ; chmod a-x $^
	
$(EXECNAME): $(OBJS)

clean:
	- rm -r $(EXECNAME).app
	- rm $(EXECNAME) links_done $(OBJS) $(SHADERS) $(OUTPUTFILE)

run:
	open -W -a $(EXECNAME)
	cat $(OUTPUTFILE)
	
