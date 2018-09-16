#LIB=$(wildcard lib/*)
LIB=lib/
INCLUDE_PATH=src/

build:
	-mkdir -p bin
	gcc test/main.c $(wildcard obj/*) -I$(INCLUDE_PATH) -o bin/test

clean:
	rm -rf obj/ bin/

ifndef VERBOSE
.SILENT:
endif

