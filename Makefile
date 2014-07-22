.PHONY: build examples

all: build

build:
	mkdir -p build
	cc src/simple2d.c -c -o build/simple2d.o
	cp simple2d-config.sh build/simple2d-config
	chmod 0777 build/simple2d-config
	ar -vq build/libsimple2d.a build/simple2d.o
	rm build/simple2d.o

install:
	cp include/simple2d.h /usr/local/include/
	cp build/libsimple2d.a /usr/local/lib/
	cp build/simple2d-config /usr/local/bin/

clean:
	rm build/libsimple2d.a
	rm build/simple2d-config

uninstall:
	rm /usr/local/include/simple2d.h
	rm /usr/local/lib/libsimple2d.a
	rm /usr/local/bin/simple2d-config

examples:
	cc -o examples/test examples/test.c `simple2d-config`

testing:
	cc src/simple2d.c -c -o build/simple2d.o
	ar -vq build/libsimple2d.a build/simple2d.o
	cc -o examples/test examples/test.c build/libsimple2d.a -Iinclude `sdl2-config --static-libs` -lSDL2_image -lSDL2_mixer -lSDL2_ttf
