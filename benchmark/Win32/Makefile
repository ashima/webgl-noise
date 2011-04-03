# Makefile for bare-bones Mingw32 from a Windows command shell (no bash, no Cygwin)
MINGW32 = C:/Dev-Cpp
#MINGW32 = C:/mingw
CC = gcc.exe
SRC = noisebench.c
SHADERS = noisebench.vert constant.frag simplexnoise2D.frag simplexnoise3D.frag\
 simplexnoise4D.frag classicnoise2D.frag classicnoise3D.frag classicnoise4D.frag
OBJ = noisebench.o
LINKOBJ = noisebench.o
LIBS = -L$(MINGW32)/lib -mwindows -lglfw -lopengl32 -lglu32 -mconsole -g3
INCS = -I. -I$(MINGW32)/include
CFLAGS = $(INCS) -Wall -O3 -ffast-math -g3
EXECNAME = noisebench.exe
OUTPUTFILE = ashimanoise.log

all: $(EXECNAME)

clean:
	del $(OBJ) $(EXECNAME) $(SHADERS) $(OUTPUTFILE) $(SRC)

noisebench.vert:
	copy ..\common\noisebench.vert .

constant.frag:
	copy ..\common\constant.frag .

simplexnoise2D.frag:
	copy ..\common\simplexnoise2D.frag .

simplexnoise3D.frag:
	copy ..\common\simplexnoise3D.frag .

simplexnoise4D.frag:
	copy ..\common\simplexnoise4D.frag .

classicnoise2D.frag:
	copy ..\common\classicnoise2D.frag .

classicnoise3D.frag:
	copy ..\common\classicnoise3D.frag .

classicnoise4D.frag:
	copy ..\common\classicnoise4D.frag .

$(SRC):
	copy ..\common\$(SRC) .

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ) $(CFLAGS)

$(EXECNAME): $(OBJ) $(SHADERS)
	$(CC) $(OBJ) -o $(EXECNAME) $(LIBS)

run: $(EXECNAME)
	./$(EXECNAME)
	type $(OUTPUTFILE)
