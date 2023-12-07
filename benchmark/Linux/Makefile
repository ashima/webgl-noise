SHADERS=noisebench.vert simplexnoise2D.frag simplexnoise3D.frag\
	simplexnoise4D.frag classicnoise2D.frag classicnoise3D.frag\
	classicnoise4D.frag constant.frag
COMDIR=../common
VPATH=$(COMDIR)
EXECNAME=noisebench
OUTPUTFILE=ashimanoise.log

all: $(EXECNAME) links_done

$(EXECNAME): noisebench.c
	clang -I. -I/usr/X11/include $^ -lglfw -lGLU -lGL -o $@

install:
	curl -o libglfw2.deb https://launchpad.net/ubuntu/+source/glfw/2.7.9-1/+build/7642129/+files/libglfw2_2.7.9-1_amd64.deb;
	curl -o libglfw2-dev.deb https://launchpad.net/ubuntu/+source/glfw/2.7.9-1/+build/7642129/+files/libglfw-dev_2.7.9-1_amd64.deb
	sudo dpkg -i libglfw2.deb libglfw2-dev.deb
	sudo apt install clang -y --no-install-recommends

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

clean:
	- rm $(EXECNAME) $(SHADERS) links_done $(OUTPUTFILE)

run:
	./$(EXECNAME)
	cat $(OUTPUTFILE)
