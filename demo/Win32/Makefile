# Makefile for bare-bones Mingw32 from a Windows command shell (no bash, no Cygwin)
MINGW32 = C:/Dev-Cpp
#MINGW32 = C:/mingw
CC = gcc.exe
SRC = noisedemo.c
SHADERS = noisedemo.vert noisedemo.frag
OBJ = noisedemo.o
LINKOBJ = noisedemo.o
LIBS = -L$(MINGW32)/lib -mwindows -lglfw -lopengl32 -lglu32 -mconsole -g3
INCS = -I. -I$(MINGW32)/include
CFLAGS = $(INCS) -Wall -O3 -ffast-math -g3
EXECNAME = noisedemo.exe

all: $(EXECNAME)

clean:
	del $(OBJ) $(EXECNAME) $(SHADERS) $(OUTPUTFILE) $(SRC)

noisedemo.vert:
	copy ..\common\noisedemo.vert .

noisedemo.frag:
	copy ..\common\noisedemo.frag .

$(SRC):
	copy ..\common\$(SRC) .

$(OBJ): $(SRC)
	$(CC) -c $(SRC) -o $(OBJ) $(CFLAGS)

$(EXECNAME): $(OBJ) $(SHADERS)
	$(CC) $(OBJ) -o $(EXECNAME) $(LIBS)

run: $(EXECNAME)
	./$(EXECNAME)
