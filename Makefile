CFLAGS=-std=c99

all: build

build:
	mkdir -p build
	cc $(CFLAGS) src/simple2d.c -c -o build/simple2d.o
	cp simple2d.sh build/simple2d
	chmod 0777 build/simple2d
	ar -vq build/libsimple2d.a build/simple2d.o
	rm build/simple2d.o

install:
	cp include/simple2d.h /usr/local/include/
	cp build/libsimple2d.a /usr/local/lib/
	cp build/simple2d /usr/local/bin/

clean:
	rm build/libsimple2d.a
	rm build/simple2d
	rm tests/testcard

uninstall:
	rm /usr/local/include/simple2d.h
	rm /usr/local/lib/libsimple2d.a
	rm /usr/local/bin/simple2d

tests:
	cc $(CFLAGS) -o tests/testcard tests/testcard.c `simple2d --libs`

.PHONY: build tests
