# Makefile for smooth v0.8

include Makefile-directories
include Makefile-options

HEADERS = $(OBJECTDIR)/smooth.h.gch

OBJECTS = classes/*/*.o classes/*/*/*.o classes/*/*/*/*.o classes/*/*/*/*/*.o misc/*.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources/*.o
endif

ifeq ($(BUILD_X64),True)
	LIBS = 
else
ifeq ($(BUILD_WIN32),True)
	LIBS = -lunicows
endif
endif

LIBS += -lfribidi -lbz2 -lxml2 -lcpuid -ljpeg -lpng -lz -lstdc++

ifeq ($(BUILD_WIN32),True)
	LIBS += -lnsucd -lws2_32 -limm32 -lole32

ifeq ($(BUILD_GDIPLUS),True)
	LIBS += -lgdiplus
endif

ifeq ($(BUILD_CAIRO),True)
	LIBS += -lcairo.dll
endif
else
	LIBS += -lcairo -lpango-1.0 -lpangocairo-1.0 -lgobject-2.0 -lpthread -lX11 -lXmu

ifeq ($(BUILD_OSX),True)
	LIBS += -lnsucd -liconv
else
	LIBS += -lgtk-x11-2.0
endif

ifeq ($(BUILD_LINUX),True)
	LIBS += -lnsucd
endif

ifeq ($(BUILD_FREEBSD),True)
	LIBS += -lrt
endif
endif

ifeq ($(BUILD_WIN32),True)
	DLLNAME = $(BINDIR)/smooth$(SHARED)
	LIBNAME = $(LIBDIR)/libsmooth.a
else
	DLLNAME = $(LIBDIR)/libsmooth$(SHARED)
endif

LINKER = gcc
STRIP = strip
REMOVER = rm
LINKER_OPTS = -L$(LIBDIR) --shared -o $(DLLNAME)
STRIP_OPTS = --strip-all
LIBSTRIP_OPTS = --strip-debug
REMOVER_OPTS = -f

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += -mwindows -Wl,--dynamicbase,--nxcompat,--kill-at,--out-implib,$(LIBNAME)
endif

ifeq ($(BUILD_FREEBSD),True)
	LINKER_OPTS += -L/usr/local/lib
endif

ifeq ($(BUILD_OSX),True)
	STRIP = true

	LINKER_OPTS += -framework Cocoa -L/opt/local/lib -L/usr/X11/lib -Wl,-dylib_install_name,libsmooth$(SHARED)
endif

.PHONY: all headers objects programs libs install clean clean_all clean_headers doc doc-clean

all: $(DLLNAME)

headers: clean_headers $(HEADERS)

objects:
	$(MAKE) -C classes
	$(MAKE) -C misc
	$(MAKE) -C resources

programs: $(DLLNAME)
	$(MAKE) -C tools
	$(MAKE) -C samples

libs:
	$(MAKE) -C libraries

install:

clean: clean_headers
	$(MAKE) -C classes clean
	$(MAKE) -C misc clean
	$(MAKE) -C resources clean
	$(MAKE) -C tools clean
	$(MAKE) -C samples clean
	$(REMOVER) $(REMOVER_OPTS) $(DLLNAME) $(LIBNAME)

clean_all: clean
	$(MAKE) -C libraries clean

clean_headers:
	$(REMOVER) $(REMOVER_OPTS) $(HEADERS)

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(DLLNAME): objects libs
	$(LINKER) $(OBJECTS) $(LINKER_OPTS) $(LIBS)
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
ifeq ($(BUILD_WIN32),True)
	$(STRIP) $(LIBSTRIP_OPTS) $(LIBNAME)
	countbuild BuildNumber
endif
