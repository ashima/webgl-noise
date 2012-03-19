SHADERS=noisedemo.vert noisedemo.frag
COMDIR=../common
VPATH=$(COMDIR)
EXECNAME=noisedemo

all: $(EXECNAME) links_done

$(EXECNAME): noisedemo.c
	gcc -I. -I/usr/X11/include $^ -lglfw -lGLU -lGL -lm -o $@

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

clean:
	- rm $(EXECNAME) $(SHADERS) links_done

run:
	./$(EXECNAME)
