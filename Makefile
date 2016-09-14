PREFIX?=/usr/local
CFLAGS=-std=c99

# Install directory and filename for the MinGW Windows installer
INSTALLER_DIR=build/win-installer-mingw
INSTALLER_FNAME=simple2d-windows-mingw.zip

BOLD=\033[1;39m
TASK=\033[1;34m
NORMAL=\033[0m

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

define task
	printf "\n${TASK}==>${BOLD} $(1) ${NORMAL}\n\n"
endef

define run_test
	$(call task,Running $(1).c)
	$(if $(MINGW), cd tests/; $(1).exe, cd tests/ ; ./$(1))
endef


all: build

build: install-deps
	@$(call task,Building)
	mkdir -p build
	cp bin/simple2d.sh build/simple2d
	chmod 0777 build/simple2d
	$(CC) $(CFLAGS) $(INCLUDES) src/simple2d.c -c -o build/simple2d.o
	$(CC) $(CFLAGS) $(INCLUDES) src/gl.c -c -o build/gl.o
ifeq ($(PLATFORM),arm)
	$(CC) $(CFLAGS) $(INCLUDES) src/gles.c -c -o build/gles.o
	ar -vq build/libsimple2d.a build/simple2d.o build/gl.o build/gles.o
	rm build/gl.o build/gles.o
else
	$(CC) $(CFLAGS) $(INCLUDES) src/gl2.c -c -o build/gl2.o
	$(CC) $(CFLAGS) $(INCLUDES) src/gl3.c -c -o build/gl3.o
	ar -vq build/libsimple2d.a build/simple2d.o build/gl.o build/gl2.o build/gl3.o
	rm build/gl.o build/gl2.o build/gl3.o
endif
	rm build/simple2d.o

install:
	@$(call task,Installing Simple 2D)
	cp include/simple2d.h      $(PREFIX)/include/
	cp build/libsimple2d.a     $(PREFIX)/lib/
	cp build/simple2d          $(PREFIX)/bin/

install-deps:
ifeq ($(PLATFORM),mingw)
	@$(call task,Installing dependencies for MinGW)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	mkdir -p $(PREFIX)/bin/
	cp -R deps/mingw/include/* $(PREFIX)/include
	cp -R deps/mingw/lib/*     $(PREFIX)/lib
	cp -R deps/mingw/bin/*     $(PREFIX)/bin
endif

ifeq ($(PLATFORM),mingw)
installer: clean build
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
	@$(call task,Cleaning)
	rm -f build/libsimple2d.a
	rm -f build/simple2d
ifeq ($(PLATFORM),mingw)
	rm -rf $(INSTALLER_DIR)
	rm -f build/$(INSTALLER_FNAME)
	rm -f tests/auto.exe
	rm -f tests/triangle.exe
	rm -f tests/testcard.exe
	rm -f tests/audio.exe
else
	rm -f tests/auto
	rm -f tests/triangle
	rm -f tests/testcard
	rm -f tests/audio
endif

uninstall:
	@$(call task,Uninstalling)
	rm -f /usr/local/include/simple2d.h
	rm -f /usr/local/lib/libsimple2d.a
	rm -f /usr/local/bin/simple2d

tests:
	@$(call task,Building tests)
	$(CC) $(CFLAGS) tests/auto.c     `simple2d --libs` -o tests/auto
	$(CC) $(CFLAGS) tests/triangle.c `simple2d --libs` -o tests/triangle
	$(CC) $(CFLAGS) tests/testcard.c `simple2d --libs` -o tests/testcard
	$(CC) $(CFLAGS) tests/audio.c    `simple2d --libs` -o tests/audio

rebuild: uninstall clean build install tests

auto:
	@$(call run_test,auto)

triangle:
	@$(call run_test,triangle)

testcard:
	@$(call run_test,testcard)

audio:
	@$(call run_test,audio)

.PHONY: build tests
