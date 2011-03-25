WINMAKE = mingw32-make

.PHONY: default clean Linux MacOSX Win32 clean-Win32

default:
	@echo "Usage:"
	@echo "make [ Linux | MacOSX | Win32 | clean | clean-Win32]"

clean:
	cd common ; make clean
	cd Linux ; make clean
	cd MacOSX ; make clean

Linux:
	cd common ; make
	cd $@ ; make ; make run

MacOSX:
	cd common ; make
	cd $@ ; make ; make run

Win32:
	cd common && $(WINMAKE)
	cd $@ && $(WINMAKE) && $(WINMAKE) run

clean-Win32:
	cd common && $(WINMAKE) clean
	cd Win32 && $(WINMAKE) clean
