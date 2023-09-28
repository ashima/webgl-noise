SHADERS=noisedemo.vert noisedemo.frag
COMDIR=../common
VPATH=$(COMDIR)
EXECNAME=noisedemo

all: $(EXECNAME) links_done

install:
	curl -o libglfw2.deb https://launchpad.net/ubuntu/+source/glfw/2.7.9-1/+build/7642129/+files/libglfw2_2.7.9-1_amd64.deb
        curl -o libglfw2-dev.deb https://launchpad.net/ubuntu/+source/glfw/2.7.9-1/+build/7642129/+files/libglfw-dev_2.7.9-1_amd64.deb
	sudo dpkg -i libglfw2.deb libglfw2-dev.deb
	sudo apt install clang -y --no-install-recommends

$(EXECNAME): noisedemo.c
	gcc -I. -I/usr/X11/include $^ -lglfw -lGLU -lGL -lm -o $@

links_done: $(SHADERS)
	ln -s $? . ; touch links_done

clean:
	- rm $(EXECNAME) $(SHADERS) links_done

run:
	./$(EXECNAME)
