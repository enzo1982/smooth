# Makefile for smooth v0.8, Windows, MinGW32

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

LIBS += -lfribidi -lbz2 -llisa -lxml2 -lcpuid -ljpeg -lpng -lz -lstdc++

ifeq ($(BUILD_WIN32),True)
	LIBS += -lws2_32 -limm32 -lole32

ifeq ($(BUILD_GDIPLUS),True)
	LIBS += -lgdiplus
endif

ifeq ($(BUILD_CAIRO),True)
	LIBS += -lcairo.dll
endif
endif

ifeq ($(BUILD_WIN32),True)
	DLLNAME = $(BINDIR)/smooth.dll
	LIBNAME = $(LIBDIR)/libsmooth.a

	LIBICONV = $(LIBDIR)/libiconv.a
	LIBICONVDLL = $(BINDIR)/iconv.dll
else
	DLLNAME = $(LIBDIR)/libsmooth.so
endif

OTHERLIBS = libs $(LIBICONVDLL)

LINKER = gcc
STRIP = strip
REMOVER = rm
LINKER_OPTS = -L$(LIBDIR) --shared -o $(DLLNAME)
STRIP_OPTS = --strip-all
LIBSTRIP_OPTS = --strip-debug
REMOVER_OPTS = -f

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += -mwindows -Xlinker --dynamicbase -Xlinker --nxcompat -Xlinker --kill-at -Xlinker --out-implib -Xlinker $(LIBNAME)
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
	$(REMOVER) $(REMOVER_OPTS) $(LIBICONVDLL)

clean_headers:
	$(REMOVER) $(REMOVER_OPTS) $(HEADERS)

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(DLLNAME): objects libs $(LIBICONVDLL)
	$(LINKER) $(OBJECTS) $(LINKER_OPTS) $(LIBS)
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
ifeq ($(BUILD_WIN32),True)
	$(STRIP) $(LIBSTRIP_OPTS) $(LIBNAME)
	countbuild BuildNumber
endif

$(LIBICONVDLL): $(LIBICONV)
ifeq ($(BUILD_WIN32),True)
	$(LINKER) libraries/libiconv/*.o -L$(LIBDIR) --shared -Xlinker --dynamicbase -Xlinker --nxcompat -o $(LIBICONVDLL)
else
	$(LINKER) libraries/libiconv/*.o -L$(LIBDIR) --shared -o $(LIBICONVDLL)
endif
	$(STRIP) $(STRIP_OPTS) $(LIBICONVDLL)
