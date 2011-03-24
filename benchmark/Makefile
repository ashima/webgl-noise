.PHONY: default clean Linux MacOSX Win32 clean-Win32

default:
	@echo "Usage:"
	@echo "make [ Linux | MacOSX | Win32 ]"

clean:
	cd Linux ; make clean
	cd MacOSX ; make clean
	cd Win32 ; make clean

Linux:
	cd common ; make
	cd $@ ; make ; make run

MacOSX:
	cd common ; make
	cd $@ ; make ; make run

Win32:
	cd common && mingw32-make
	cd $@ && mingw32-make && mingw32-make run

clean-Win32:
	cd Win32 && mingw32-make clean
