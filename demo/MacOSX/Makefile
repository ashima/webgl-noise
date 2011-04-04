EXECNAME=noisedemo
COMDIR=../common

OBJS=noisedemo.o
SHADERS=noisedemo.vert noisedemo.frag

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
	- rm $(EXECNAME) links_done $(OBJS) $(SHADERS)

run:
	open -W ./$(EXECNAME).app
