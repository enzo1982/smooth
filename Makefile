# Makefile for smooth v0.8

include Makefile-directories
include Makefile-options

HEADERS = $(OBJECTDIR)/smooth.h.gch

OBJECTS = classes/*/*.o classes/*/*/*.o classes/*/*/*/*.o classes/*/*/*/*/*.o misc/*.o

LIBS = -lfribidi -lbz2 -lxml2 -ljpeg -lz -lstdc++

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources/*.o

	ifneq ($(BUILD_X64),True)
		LIBS += -lunicows
	endif

	ifeq ($(BUILD_GDIPLUS),True)
		LIBS += -lgdiplus
	endif

	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo.dll
	endif

	LIBS += -lcpuid -lnsucd -lpng -lz -lws2_32 -limm32 -lole32

	DLLNAME = $(BINDIR)/smooth$(SHARED)
	LIBNAME = $(LIBDIR)/libsmooth.a
else ifeq ($(BUILD_OSX),True)
	LIBS += -lcpuid -lnsucd -liconv -lpng -lcairo -lpthread -lX11 -lXmu

	DLLNAME = $(LIBDIR)/libsmooth$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lpng -lbe -ltracker

	DLLNAME = $(LIBDIR)/libsmooth$(SHARED)
else ifeq ($(BUILD_QNX),True)
	LIBS += -liconv -lpng -lz -lsocket -lX11 -lXmu -lph

	DLLNAME = $(LIBDIR)/libsmooth$(SHARED)
else
	ifeq ($(BUILD_LINUX),True)
		LIBS += -lcpuid -lnsucd
	else ifeq ($(BUILD_FREEBSD),True)
		LIBS += -lrt
	else ifeq ($(BUILD_OPENBSD),True)
		LIBS += -lXau -lXdmcp -lXxf86vm -lSM -lICE -lffi -ldrm -lpcre
	endif

	LIBS += $(shell pkg-config --libs xmu) $(shell pkg-config --libs gtk+-2.0)
	LIBS += -lpthread

	DLLNAME = $(LIBDIR)/libsmooth$(SHARED)
endif

LINKER	      = gcc
STRIP	      = strip
REMOVER	      = rm
LINKER_OPTS   = -L$(LIBDIR) --shared -o $(DLLNAME)
STRIP_OPTS    = 
LIBSTRIP_OPTS = 
REMOVER_OPTS  = -f

ifeq ($(BUILD_X64),True)
	LINKER_OPTS += -m64
else
	LINKER_OPTS += -m32
endif

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += -mwindows -Wl,--dynamicbase,--nxcompat,--kill-at,--out-implib,$(LIBNAME)
else ifeq ($(BUILD_FREEBSD),True)
	LINKER_OPTS += -L/usr/local/lib
else ifeq ($(BUILD_OPENBSD),True)
	LINKER_OPTS += -Wl,-rpath,/usr/local/lib -L/usr/local/lib -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib
else ifeq ($(BUILD_NETBSD),True)
	LINKER_OPTS += -Wl,-rpath,/usr/pkg/lib -L/usr/pkg/lib -Wl,-rpath,/usr/X11R7/lib -L/usr/X11R7/lib
else ifeq ($(BUILD_HAIKU),True)
	LINKER_OPTS += -L/boot/common/lib
else ifeq ($(BUILD_QNX),True)
	LINKER_OPTS += -L/usr/X11R6/lib
else ifeq ($(BUILD_OSX),True)
	LINKER_OPTS += -framework Cocoa -L/usr/X11/lib -Wl,-dylib_install_name,libsmooth$(SHARED)
endif

ifneq ($(BUILD_SOLARIS),True)
	STRIP_OPTS    += --strip-all
	LIBSTRIP_OPTS += --strip-debug
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
ifneq ($(BUILD_OSX),True)
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
endif
ifeq ($(BUILD_WIN32),True)
	$(STRIP) $(LIBSTRIP_OPTS) $(LIBNAME)
	countbuild BuildNumber
endif
