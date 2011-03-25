# Makefile for bare-bones Mingw32 from a Windows command shell (no bash, no Cygwin)
MINGW32 = C:/Dev-Cpp
#MINGW32 = C:/mingw
CC = gcc.exe
SRC = noisebench.c
SHADERS = GLSL-ashimanoise.vert GLSL-ashimanoise2D.frag GLSL-ashimanoise3D.frag GLSL-ashimanoise4D.frag
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

GLSL-ashimanoise.vert:
	copy ..\common\GLSL-ashimanoise.vert .

GLSL-ashimanoise2D.frag:
	copy ..\common\GLSL-ashimanoise2D.frag .

GLSL-ashimanoise3D.frag:
	copy ..\common\GLSL-ashimanoise3D.frag .

GLSL-ashimanoise4D.frag:
	copy ..\common\GLSL-ashimanoise4D.frag .

$(SRC):
	copy ..\common\$(SRC) .

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ) $(CFLAGS)

$(EXECNAME): $(OBJ) $(SHADERS)
	$(CC) $(OBJ) -o $(EXECNAME) $(LIBS)

run: $(EXECNAME)
	./$(EXECNAME)
	type $(OUTPUTFILE)
