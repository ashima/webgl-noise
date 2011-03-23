SHADERS=GLSL-ashimanoise.vert GLSL-ashimanoise2D.frag \
	GLSL-ashimanoise3D.frag GLSL-ashimanoise4D.frag
COMDIR=../common
VPATH=$(COMDIR)

OBJS=noisebench.o

CFLAGS=-I. -I/usr/X11/include
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw

all: noisebench.app links_done

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

noisebench.app: noisebench
	./bundle.sh $@ $^ ; chmod a-x $^
	
noisebench: $(OBJS)

clean:
	rm -r noisebench.app; rm noisebench links_done $(OBJS) $(SHADERS)


