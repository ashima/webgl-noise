EXECNAME=noisebench
OUTPUTFILE=ashimanoise.log
COMDIR=../common

OBJS=noisebench.o
SHADERS=noisebench.vert simplexnoise2D.frag simplexnoise3D.frag\
	simplexnoise4D.frag classicnoise2D.frag classicnoise3D.frag\
	classicnoise4D.frag constant.frag

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
	open -W ./$(EXECNAME).app
	cat $(OUTPUTFILE)
