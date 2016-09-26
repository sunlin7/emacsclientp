.PHONY: all clean install
all: clean
	windres -O coff -o emacsclientp.res emacsclientp.rc
	gcc -mwindows emacsclientp.res emacsclientp.c -o emacsclientp.exe
install: all
	cp emacsclientp.exe $(dir $(shell which emacsclient))
clean:
	-rm *.res *.exe
