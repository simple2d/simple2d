# Simple 2D Makefile for Unix-like systems
# (macOS, Linux, Windows/MinGW, Raspberry Pi OS)
################################################################################

PREFIX?=/usr/local
PKG_CONFIG := $(shell command -v pkg-config)
ARCH := $(shell uname -m)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	ARCH = universal
	PLATFORM = macos
	MACOS_FRAMEWORKS = \
	  AppKit \
		AVFoundation \
		AudioToolbox \
		Carbon \
		Cocoa \
		CoreAudio \
		CoreHaptics \
		CoreMedia \
		ForceFeedback \
		GameController \
		IOKit \
		Metal \
		OpenGL \
		QuartzCore \
		UniformTypeIdentifiers
	PLATFORM_LIBS += $(foreach fw,$(MACOS_FRAMEWORKS),-framework $(fw))
else ifeq ($(UNAME_S),Linux)
	PLATFORM = linux
	PLATFORM_LIBS += -lm
else ifneq (,$(findstring MINGW,$(UNAME_S)))
	PLATFORM = mingw
	PLATFORM_LIBS += -lhid -lsetupapi -lole32 -loleaut32 -luuid -lgdi32 -luser32 -lwinmm -lusp10 -limm32 -lversion -lrpcrt4
endif

CFLAGS += -I./deps/include -DS2D_NO_SDL_MAIN
SOURCES=$(notdir $(wildcard src/*.c))
OBJECTS=$(addprefix build/,$(notdir $(SOURCES:.c=.o)))
LIBS += -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf $(PLATFORM_LIBS)

CFLAGS_TEST=-I./include -I./deps/include
SOURCES_TEST=$(notdir $(wildcard test/*.c))
OBJECTS_TEST=$(addprefix test/,$(notdir $(SOURCES_TEST:.c=.o)))
LDFLAGS_TEST=-L./build -lsimple2d -L./deps/$(PLATFORM)/$(ARCH) $(LIBS)


# Helper Functions
################################################################################

define task_msg
	@printf "\n\033[1;34m==>\033[39m $(1)\033[0m\n\n"
endef

define info_msg
	@printf "\033[1;36mInfo:\e[39m $(1)\033[0m\n"
endef


# Targets
################################################################################

# Build the Simple 2D library (default target)
all: clean build $(SOURCES)
	ar -vq build/libsimple2d.a $(OBJECTS)
	rm build/*.o

# Compile each source file into an object file
$(SOURCES):
	$(CC) $(CFLAGS) src/$@ -c -o build/$(basename $@).o

# Prepare the build directory and check for dependencies
build:
	$(call task_msg,Building Simple 2D library)
	@if [ ! -d deps/include/SDL3 ] || [ ! -d deps/include/SDL3_image ] || [ ! -d deps/include/SDL3_mixer ] || [ ! -d deps/include/SDL3_ttf ]; then \
		printf "\033[1;31mError:\e[39m SDL3 is missing from deps/include. Run \`make deps\` to build dependencies.\033[0m\n"; \
		exit 1; \
	fi
	mkdir -p build

# Remove build and test artifacts
clean:
	$(call task_msg,Cleaning)
	rm -rf build/*
	rm -f test/auto
	rm -f test/triangle
	rm -f test/testcard
	rm -f test/sounds
	rm -f test/controller

# Build and update all dependencies
deps: deps-clean deps-update deps-build

# Remove all built dependencies
deps-clean:
	$(call task_msg,Cleaning dependencies)
	rm -rf deps/build/*
	rm -rf deps/include/*
	rm -rf deps/$(PLATFORM)/$(ARCH)/*

# Update dependency sources (git submodules)
deps-update:
	$(call task_msg,Updating dependencies)
	git submodule update --recursive --remote
	# Checkout last good commit if needed, for example:
	#   (cd deps/SDL_mixer && git checkout 4e9a308a0daa63b6bd492b0ec616f86337500898)

# Build dependencies using CMake and copy headers/libs
deps-build:
	@if ! command -v cmake >/dev/null 2>&1; then \
		printf "\033[1;31mError:\e[39m CMake is not installed or not found in PATH. Please install CMake to build dependencies.\033[0m\n"; \
		exit 1; \
	fi
	@if [ ! -d deps/SDL ] || [ ! -d deps/SDL_image ] || [ ! -d deps/SDL_mixer ] || [ ! -d deps/SDL_ttf ]; then \
	  printf "\033[1;31mError:\e[39m Missing SDL3 libraries. Run \`make deps-update\` to download dependencies.\033[0m\n"; \
		exit 1; \
	fi
	$(call task_msg,Building dependencies)
	cd deps && cmake -S . -B build && cmake --build build --parallel
	mkdir -p deps/include
	mkdir -p deps/$(PLATFORM)/$(ARCH)
	cp -r deps/SDL/include/SDL3 deps/include
	cp -r deps/SDL_image/include/SDL3_image deps/include
	cp -r deps/SDL_mixer/include/SDL3_mixer deps/include
	cp -r deps/SDL_ttf/include/SDL3_ttf deps/include
	cp deps/build/SDL/libSDL3.a deps/$(PLATFORM)/$(ARCH)
	cp deps/build/SDL_image/libSDL3_image.a deps/$(PLATFORM)/$(ARCH)
	cp deps/build/SDL_mixer/libSDL3_mixer.a deps/$(PLATFORM)/$(ARCH)
	cp deps/build/SDL_ttf/libSDL3_ttf.a deps/$(PLATFORM)/$(ARCH)
	$(call info_msg,Run \`make deps-test\` to test SDL dependencies)

# Test SDL dependencies by compiling and running a test program
deps-test:
	rm -f deps/test
	cc -I./deps/include deps/test.c -L./deps/$(PLATFORM)/$(ARCH) $(LIBS) -o deps/test
	./deps/test

# Build and run all tests
test: build-tests $(SOURCES_TEST)

# Build test binaries
build-tests:
	$(call task_msg,Building tests)

# Compile each test source file into a binary
$(SOURCES_TEST):
	$(CC) $(CFLAGS_TEST) test/$@ $(LDFLAGS_TEST) -o test/$(basename $@)

# Helper macro to run a test binary with diagnostics
define run_test
	$(call task_msg,Running $(1).c)
	@cd test/; ./$(1) --diagnostics
endef

# Run the `auto` test
run_auto:
	$(call run_test,auto)

# Run the `triangle` test
run_triangle:
	$(call run_test,triangle)

# Run the `testcard` test
run_testcard:
	$(call run_test,testcard)

# Run the `sounds` test
run_sounds:
	$(call run_test,sounds)

# Run the `controller` test
run_controller:
	$(call run_test,controller)

# Show help for available tasks
help:
	$(call info_msg,Available tasks)
	@echo "all            Build the Simple 2D library"
	@echo "deps           Build and update all dependencies"
	@echo "deps-clean     Remove all built dependencies"
	@echo "deps-update    Update dependency sources"
	@echo "deps-build     Build dependencies"
	@echo "deps-test      Test SDL dependencies"
	@echo "clean          Clean build and test artifacts"
	@echo "test           Build all tests"
	@echo "run_auto       Run the \`auto\` test"
	@echo "run_triangle   Run the \`triangle\` test"
	@echo "run_testcard   Run the \`testcard\` test"
	@echo "run_sounds     Run the \`sounds\` test"
	@echo "run_controller Run the \`controller\` test"

.PHONY: all clean deps deps-clean deps-update deps-build deps-test test build-tests \
       run_auto run_triangle run_testcard run_sounds run_controller help
