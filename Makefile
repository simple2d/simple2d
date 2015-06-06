CFLAGS=-std=c99

# Includes and libs for Raspberry Pi
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),armv6l)
  PLATFORM=rpi
  INCLUDES=-I/opt/vc/include/
endif
ifeq ($(UNAME_M),armv7l)
  PLATFORM=rpi
  INCLUDES=-I/opt/vc/include/
endif

all: build

build:
	mkdir -p build
	cp simple2d.sh build/simple2d
	chmod 0777 build/simple2d
	cc $(CFLAGS) $(INCLUDES) src/simple2d.c -c -o build/simple2d.o
	cc $(CFLAGS) $(INCLUDES) src/gl.c -c -o build/gl.o
ifeq ($(PLATFORM),rpi)
	cc $(CFLAGS) $(INCLUDES) src/gles.c -c -o build/gles.o
	ar -vq build/libsimple2d.a build/simple2d.o build/gl.o build/gles.o
	rm build/gles.o
else
	cc $(CFLAGS) $(INCLUDES) src/gl2.c -c -o build/gl2.o
	cc $(CFLAGS) $(INCLUDES) src/gl3.c -c -o build/gl3.o
	ar -vq build/libsimple2d.a build/simple2d.o build/gl.o build/gl2.o build/gl3.o
	rm build/gl.o build/gl2.o build/gl3.o
endif
	rm build/simple2d.o

install:
	cp include/simple2d.h /usr/local/include/
	cp build/libsimple2d.a /usr/local/lib/
	cp build/simple2d /usr/local/bin/

clean:
	rm -f build/libsimple2d.a
	rm -f build/simple2d
	rm -f tests/audio
	rm -f tests/testcard
	rm -f tests/triangle

uninstall:
	rm -f /usr/local/include/simple2d.h
	rm -f /usr/local/lib/libsimple2d.a
	rm -f /usr/local/bin/simple2d

tests:
	cc $(CFLAGS) tests/audio.c `simple2d --libs` -o tests/audio
	cc $(CFLAGS) tests/testcard.c `simple2d --libs` -o tests/testcard
	cc $(CFLAGS) tests/triangle.c `simple2d --libs` -o tests/triangle

.PHONY: build tests
