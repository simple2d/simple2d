# Makefile for Unix-like systems:
#   macOS, Linux, Raspberry Pi, MinGW

# For ARM platforms
ifneq (,$(findstring arm,$(shell uname -m)))
	PLATFORM=arm
	# Raspberry Pi includes
	INCLUDES=-I/opt/vc/include/
endif

# For Linux
ifeq ($(shell uname),Linux)
	CFLAGS+=-fPIC
endif

# For MinGW
ifneq (,$(findstring MINGW,$(shell uname -s)))
	PLATFORM=mingw
	MINGW=true
	CC=gcc
	INCLUDES=-I/usr/local/include/
endif

PREFIX?=/usr/local
CFLAGS=-std=c99

SOURCES=simple2d shapes image sprite text sound music input controllers window
ifeq ($(PLATFORM),arm)
	SOURCES+=gl gles
else
	SOURCES+=gl gl2 gl3
endif

OBJECTS=$(foreach var,$(SOURCES),build/$(var).o)

# Install directory and filename for the MinGW Windows installer
INSTALLER_DIR=build/win-installer-mingw
INSTALLER_FNAME=simple2d-windows-mingw.zip

define task
	@printf "\n\033[1;34m==>\033[1;39m $(1)\033[0m\n\n"
endef

define run_test
	$(call task,Running $(1).c)
	@$(if $(MINGW), cd test/; $(1).exe, cd test/ ; ./$(1))
endef


all: prereqs install-deps $(SOURCES)
	ar -vq build/libsimple2d.a $(OBJECTS)
	cp bin/simple2d.sh build/simple2d
	chmod 0777 build/simple2d
	rm build/*.o

prereqs:
	$(call task,Building)
	mkdir -p build

$(SOURCES):
	$(CC) $(CFLAGS) $(INCLUDES) src/$@.c -c -o build/$@.o

install:
	$(call task,Installing Simple 2D)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	mkdir -p $(PREFIX)/bin/
	cp include/simple2d.h  $(PREFIX)/include/
	cp build/libsimple2d.a $(PREFIX)/lib/
	cp build/simple2d      $(PREFIX)/bin/

install-deps:
ifeq ($(PLATFORM),mingw)
	$(call task,Installing dependencies for MinGW)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	mkdir -p $(PREFIX)/bin/
	cp -R deps/mingw/include/* $(PREFIX)/include
	cp -R deps/mingw/lib/*     $(PREFIX)/lib
	cp -R deps/mingw/bin/*     $(PREFIX)/bin
endif

ifeq ($(PLATFORM),mingw)
installer: clean all
	mkdir -p $(INSTALLER_DIR)/include
	mkdir -p $(INSTALLER_DIR)/lib
	mkdir -p $(INSTALLER_DIR)/bin
	cp -R deps/mingw/include/*    $(INSTALLER_DIR)/include
	cp -R deps/mingw/lib/*        $(INSTALLER_DIR)/lib
	cp -R deps/mingw/bin/*        $(INSTALLER_DIR)/bin
	cp    deps/LICENSES.md        $(INSTALLER_DIR)
	cp include/simple2d.h         $(INSTALLER_DIR)/include
	cp build/libsimple2d.a        $(INSTALLER_DIR)/lib
	cp build/simple2d             $(INSTALLER_DIR)/bin
	cp bin/win-installer-mingw.sh $(INSTALLER_DIR)/install.sh
	PowerShell -Command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('$(INSTALLER_DIR)', 'build\$(INSTALLER_FNAME)'); }"
endif

clean:
	$(call task,Cleaning)
	rm -f build/libsimple2d.a
	rm -f build/simple2d
ifeq ($(PLATFORM),mingw)
	rm -rf $(INSTALLER_DIR)
	rm -f build/$(INSTALLER_FNAME)
	rm -f test/auto.exe
	rm -f test/triangle.exe
	rm -f test/testcard.exe
	rm -f test/audio.exe
	rm -f test/controller.exe
else
	rm -f test/auto
	rm -f test/triangle
	rm -f test/testcard
	rm -f test/audio
	rm -f test/controller
endif

uninstall:
	$(call task,Uninstalling)
	rm -f /usr/local/include/simple2d.h
	rm -f /usr/local/lib/libsimple2d.a
	rm -f /usr/local/bin/simple2d

test:
	$(call task,Building tests)
	$(CC) $(CFLAGS) test/auto.c       `simple2d --libs` -o test/auto
	$(CC) $(CFLAGS) test/triangle.c   `simple2d --libs` -o test/triangle
	$(CC) $(CFLAGS) test/testcard.c   `simple2d --libs` -o test/testcard
	$(CC) $(CFLAGS) test/audio.c      `simple2d --libs` -o test/audio
	$(CC) $(CFLAGS) test/controller.c `simple2d --libs` -o test/controller

rebuild: uninstall clean all install test

auto:
	$(call run_test,auto)

triangle:
	$(call run_test,triangle)

testcard:
	$(call run_test,testcard)

audio:
	$(call run_test,audio)

controller:
	$(call run_test,controller)

.PHONY: build test
