
.PHONY: default clean Linux MacOSX Win32

default:
	@echo Usage: ; echo 'make [Linux | MacOSX | Win32 ]'

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
	cd common ; make
	cd $@ ; make ; make run

