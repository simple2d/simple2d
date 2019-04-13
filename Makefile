# Makefile for Unix-like systems:
#   macOS, Linux, Raspberry Pi, MinGW

PREFIX?=/usr/local
CFLAGS=-std=c11

# ARM platforms
ifneq (,$(findstring arm,$(shell uname -m)))
	# Raspberry Pi includes
	INCLUDES=-I/opt/vc/include/
endif

# Apple
ifeq ($(shell uname),Darwin)
	PLATFORM=apple
	XCPRETTY_STATUS=$(shell xcpretty -v &>/dev/null; echo $$?)
	ifeq ($(XCPRETTY_STATUS),0)
		XCPRETTY=xcpretty
	else
		XCPRETTY=cat
	endif
endif

# Linux
ifeq ($(shell uname),Linux)
	CFLAGS+=-fPIC
endif

# BSD 
ifneq (,$(findstring BSD,$(shell uname)))
        CC=cc
        INCLUDES=-I/usr/local/include/
        CFLAGS+=-fPIC
endif

# MinGW
ifneq (,$(findstring MINGW,$(shell uname -s)))
	PLATFORM=mingw
	CC=gcc
	INCLUDES=-I/usr/local/include/
endif

SOURCES=$(notdir $(wildcard src/*.c))
OBJECTS=$(addprefix build/,$(notdir $(SOURCES:.c=.o)))

VERSION=$(shell bash bin/simple2d.sh -v)

# Release directories and archive filenames
APPLE_RELEASE=simple2d-apple-frameworks-$(VERSION)
MINGW_RELEASE=simple2d-windows-mingw-$(VERSION)

# Helper functions

define task_msg
	@printf "\n\033[1;34m==>\033[39m $(1)\033[0m\n\n"
endef

define info_msg
	@printf "\033[1;36mInfo:\e[39m $(1)\033[0m\n"
endef

define run_test
	$(call task_msg,Running $(1).c)
	@cd test/; ./$(1)
endef

# Targets

all: prereqs install-deps $(SOURCES)
	ar -vq build/libsimple2d.a $(OBJECTS)
	cp bin/simple2d.sh build/simple2d
	chmod 0777 build/simple2d
	rm build/*.o

prereqs:
	$(call task_msg,Building)
	mkdir -p build

install-deps:
ifeq ($(PLATFORM),mingw)
	$(call task_msg,Installing dependencies for MinGW)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	mkdir -p $(PREFIX)/bin/
	cp -R deps/mingw/include/* $(PREFIX)/include
	cp -R deps/mingw/lib/*     $(PREFIX)/lib
	cp -R deps/mingw/bin/*     $(PREFIX)/bin
endif

$(SOURCES):
	$(CC) $(CFLAGS) $(INCLUDES) src/$@ -c -o build/$(basename $@).o

install:
	$(call task_msg,Installing Simple 2D)
	mkdir -p $(PREFIX)/include/
	mkdir -p $(PREFIX)/lib/
	mkdir -p $(PREFIX)/bin/
	cp include/simple2d.h  $(PREFIX)/include/
	cp build/libsimple2d.a $(PREFIX)/lib/
	cp build/simple2d      $(PREFIX)/bin/

ifeq ($(PLATFORM),apple)
frameworks: all
	$(call task_msg,Building iOS and tvOS frameworks)
	xcodebuild -version
ifneq ($(XCPRETTY_STATUS),0)
	@echo "xcpretty not found: Run \`gem install xcpretty\` for nicer xcodebuild output.\n"
endif
	mkdir -p build
	cp -R deps/xcode/Simple2D.xcodeproj build
	cd build && \
	xcodebuild -sdk iphoneos         | $(XCPRETTY) && \
	xcodebuild -sdk iphonesimulator  | $(XCPRETTY) && \
	xcodebuild -sdk appletvos        | $(XCPRETTY) && \
	xcodebuild -sdk appletvsimulator | $(XCPRETTY)
	mkdir -p build/Release-ios-universal
	mkdir -p build/Release-tvos-universal
	lipo build/Release-iphoneos/libsimple2d.a  build/Release-iphonesimulator/libsimple2d.a  -create -output build/Release-ios-universal/libsimple2d.a
	lipo build/Release-appletvos/libsimple2d.a build/Release-appletvsimulator/libsimple2d.a -create -output build/Release-tvos-universal/libsimple2d.a
	mkdir -p build/ios build/tvos
	libtool -static build/Release-ios-universal/libsimple2d.a  deps/ios/SDL2.framework/SDL2  -o build/ios/Simple2D
	libtool -static build/Release-tvos-universal/libsimple2d.a deps/tvos/SDL2.framework/SDL2 -o build/tvos/Simple2D
	mkdir -p build/ios/Simple2D.framework/Headers
	mkdir -p build/tvos/Simple2D.framework/Headers
	cp include/simple2d.h build/ios/Simple2D.framework/Headers
	cp include/simple2d.h build/tvos/Simple2D.framework/Headers
	cp -R deps/ios/include/SDL2  build/ios/Simple2D.framework/Headers
	cp -R deps/tvos/include/SDL2 build/tvos/Simple2D.framework/Headers
	cp deps/xcode/Info.plist build/ios/Simple2D.framework/Info.plist
	cp deps/xcode/Info.plist build/tvos/Simple2D.framework/Info.plist
	mv build/ios/Simple2D  build/ios/Simple2D.framework
	mv build/tvos/Simple2D build/tvos/Simple2D.framework
	$(call info_msg,iOS framework built at \`build/ios/Simple2D.framework\`)
	$(call info_msg,tvOS framework built at \`build/tvos/Simple2D.framework\`)
endif

ifeq ($(PLATFORM),apple)
install-frameworks: install
ifeq ($(shell test -d build/ios/Simple2D.framework && test -d build/tvos/Simple2D.framework; echo $$?),1)
	$(error Frameworks missing. Run `frameworks` target first)
endif
	$(call task_msg,Installing iOS and tvOS frameworks)
	mkdir -p $(PREFIX)/Frameworks/Simple2D/iOS/
	mkdir -p $(PREFIX)/Frameworks/Simple2D/tvOS/
	cp -R build/ios/Simple2D.framework  $(PREFIX)/Frameworks/Simple2D/iOS/
	cp -R build/tvos/Simple2D.framework $(PREFIX)/Frameworks/Simple2D/tvOS/
endif

ifeq ($(PLATFORM),apple)
release: clean frameworks
	$(call task_msg,Building iOS and tvOS release)
	mkdir -p build/$(APPLE_RELEASE)/Simple2D/iOS
	mkdir -p build/$(APPLE_RELEASE)/Simple2D/tvOS
	cp -R build/ios/*  build/$(APPLE_RELEASE)/Simple2D/iOS/
	cp -R build/tvos/* build/$(APPLE_RELEASE)/Simple2D/tvOS/
	cd build; zip -rq $(APPLE_RELEASE).zip $(APPLE_RELEASE)
	$(call info_msg,Frameworks zipped at \`build/$(APPLE_RELEASE).zip\`)
endif

ifeq ($(PLATFORM),mingw)
release: clean all
	mkdir -p build/$(MINGW_RELEASE)/include
	mkdir -p build/$(MINGW_RELEASE)/lib
	mkdir -p build/$(MINGW_RELEASE)/bin
	cp -R deps/mingw/include/*    build/$(MINGW_RELEASE)/include
	cp -R deps/mingw/lib/*        build/$(MINGW_RELEASE)/lib
	cp -R deps/mingw/bin/*        build/$(MINGW_RELEASE)/bin
	cp    deps/LICENSES.md        build/$(MINGW_RELEASE)
	cp include/simple2d.h         build/$(MINGW_RELEASE)/include
	cp build/libsimple2d.a        build/$(MINGW_RELEASE)/lib
	cp build/simple2d             build/$(MINGW_RELEASE)/bin
	cp bin/win-installer-mingw.sh build/$(MINGW_RELEASE)/install.sh
	PowerShell -Command "& { Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::CreateFromDirectory('build/$(MINGW_RELEASE)', 'build/\$(MINGW_RELEASE).zip'); }"
endif

clean:
	$(call task_msg,Cleaning)
	rm -rf build/*
ifeq ($(PLATFORM),mingw)
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
	$(call task_msg,Uninstalling)
	rm -f  /usr/local/include/simple2d.h
	rm -f  /usr/local/lib/libsimple2d.a
	rm -f  /usr/local/bin/simple2d
ifeq ($(PLATFORM),apple)
	rm -rf /usr/local/Frameworks/Simple2D
endif

test:
	$(call task_msg,Building tests)
	simple2d build test/auto.c
	simple2d build test/triangle.c
	simple2d build test/testcard.c
	simple2d build test/audio.c
	simple2d build test/controller.c

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

ifeq ($(PLATFORM),apple)
ios:
ifeq ($(shell test -d /usr/local/Frameworks/Simple2D/iOS/Simple2D.framework; echo $$?),1)
	$(error Simple2D.framework missing for iOS. Run `frameworks` and `install-frameworks` targets first)
endif
	$(call task_msg,Running iOS test)
	mkdir -p build/ios
	cp -R deps/xcode/ios/* build/ios
	cp test/triangle-ios-tvos.c build/ios/main.c
	simple2d build --ios build/ios/MyApp.xcodeproj
	simple2d simulator --open "iPhone X"
	simple2d simulator --install "build/ios/build/Release-iphonesimulator/MyApp.app"
	simple2d simulator --launch "Simple2D.MyApp"
endif

ifeq ($(PLATFORM),apple)
tvos:
ifeq ($(shell test -d /usr/local/Frameworks/Simple2D/tvOS/Simple2D.framework; echo $$?),1)
	$(error Simple2D.framework missing for tvOS. Run `frameworks` and `install-frameworks` targets first)
endif
	$(call task_msg,Running tvOS test)
	mkdir -p build/tvos
	cp -R deps/xcode/tvos/* build/tvos
	cp test/triangle-ios-tvos.c build/tvos/main.c
	simple2d build --tvos build/tvos/MyApp.xcodeproj
	simple2d simulator --open "Apple TV 4K"
	simple2d simulator --install "build/tvos/build/Release-appletvsimulator/MyApp.app"
	simple2d simulator --launch "Simple2D.MyApp"
endif

.PHONY: build test
