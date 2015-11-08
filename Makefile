# Makefile for smooth v0.8

SMOOTH_PATH = .

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

### Output options ###

OBJECTS = classes/*/*.o classes/*/*/*.o classes/*/*/*/*.o classes/*/*/*/*/*.o misc/*.o

LIBS = -lfribidi -lcpuid -lbz2 -lxml2 -ljpeg -lstdc++

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources/*.o

	ifeq ($(BUILD_GDIPLUS),True)
		LIBS += -lgdiplus
	endif

	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo.dll
	endif

	LIBS += -lpng -lz -lws2_32 -limm32 -lole32 -luuid

	DLLNAME = $(BINDIR)/smooth$(SHARED)
	LIBNAME = $(LIBDIR)/libsmooth.a
else ifeq ($(BUILD_OSX),True)
	ifeq ($(BUILD_XLIB),True)
		LIBS += -lpng -lz -lX11 -lXmu -lXft
	else
		LIBS += $(LIBDIR)/libpng.a $(LIBDIR)/libz.a
	endif

	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo
	endif

	LIBS += -liconv -lpthread

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lpng -lz -lbe -ltracker

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_QNX),True)
	LIBS += -liconv -lpng -lz -lsocket -lX11 -lXmu -lXft -lph

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else
	ifeq ($(BUILD_FREEBSD),True)
		LIBS += -lrt
	else ifeq ($(BUILD_OPENBSD),True)
		LIBS += -lXau -lXdmcp -lXxf86vm -lSM -lICE -lffi -ldrm -lpcre
	endif

	LIBS += $(shell pkg-config --libs libpng) $(shell pkg-config --libs xmu) $(shell pkg-config --libs xft)
	LIBS += -lpthread

	ifeq ($(BUILD_SOLARIS),True)
		LIBS += $(shell pkg-config --libs gtk+-2.0)
	else
		LIBS += $(shell pkg-config --libs gtk+-3.0)
	endif

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
endif

### Linker options ###

LINKER	    = gcc
REMOVE	    = rm
LINKER_OPTS = -L$(LIBDIR) -o $(DLLNAME)
REMOVE_OPTS = -f

ifeq ($(BUILD_FREEBSD),True)
	LINKER = clang
endif

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += --shared -mwindows -Wl,--dynamicbase,--nxcompat,--kill-at,--out-implib,$(LIBNAME)
else ifeq ($(BUILD_OSX),True)
	LINKER_OPTS += -dynamiclib -framework Carbon -framework Cocoa -Wl,-dylib_install_name,libsmooth-$(VERSION).$(REVISION)$(SHARED)

	ifeq ($(BUILD_XLIB),True)
		LINKER_OPTS += -L/usr/X11/lib
	endif
else
	LINKER_OPTS += --shared -Wl,-soname,libsmooth-$(VERSION)$(SHARED).$(REVISION)

	ifeq ($(BUILD_FREEBSD),True)
		LINKER_OPTS += -L/usr/local/lib
	else ifeq ($(BUILD_OPENBSD),True)
		LINKER_OPTS += -Wl,-rpath,/usr/local/lib -L/usr/local/lib -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib
	else ifeq ($(BUILD_NETBSD),True)
		LINKER_OPTS += -Wl,-rpath,/usr/pkg/lib -L/usr/pkg/lib -Wl,-rpath,/usr/X11R7/lib -L/usr/X11R7/lib
	else ifeq ($(BUILD_HAIKU),True)
		LINKER_OPTS += -L/boot/common/lib
	else ifeq ($(BUILD_QNX),True)
		LINKER_OPTS += -L/usr/X11R6/lib
	endif
endif

### Install options ###
COPY  = cp
LINK  = ln
CHMOD = chmod

### Targets ###

.PHONY: all folders objects lib programs libs install uninstall clean distclean doc doc-clean

all: folders lib programs

folders:
	mkdir -p $(SMOOTH_PATH)/$(BINDIR) $(SMOOTH_PATH)/$(LIBDIR)

objects:
	$(call makein,classes)
	$(call makein,misc)
	$(call makein,resources)

lib: $(DLLNAME)

programs: lib
	$(call makein,tools)
	$(call makein,samples)

libs:
	$(call makein,libraries)

install: all
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-$(VERSION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-$(VERSION)$(SHARED).$(REVISION) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif

ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_HAIKU),True)
ifneq ($(BUILD_QNX),True)
	/sbin/ldconfig 2> /dev/null || true
endif
endif
endif
else
	$(INSTALL) -d $(DESTDIR)$(libdir)
	$(INSTALL_DATA) $(DLLNAME) $(DESTDIR)$(libdir)/libsmooth-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-$(VERSION).$(REVISION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif
endif

	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) $(BINDIR)/translator $(DESTDIR)$(bindir)

	$(INSTALL) -d $(DESTDIR)$(includedir)
	$(COPY) -r $(SRCDIR)/include/smooth $(DESTDIR)$(includedir)
	$(CHMOD) -R a=rX,u=rwX $(DESTDIR)$(includedir)/smooth

ifeq ($(BUILD_V8),True)
	$(COPY) -r $(SRCDIR)/include/smooth-js $(DESTDIR)$(includedir)
	$(CHMOD) -R a=rX,u=rwX $(DESTDIR)$(includedir)/smooth-js
endif

	$(INSTALL_DATA) $(SRCDIR)/include/smooth.h $(DESTDIR)$(includedir)
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED).$(REVISION)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
endif

ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_HAIKU),True)
ifneq ($(BUILD_QNX),True)
	/sbin/ldconfig 2> /dev/null || true
endif
endif
endif
else
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-$(VERSION).$(REVISION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
endif
endif

	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(bindir)/translator

	$(REMOVE) $(REMOVE_OPTS) -r $(DESTDIR)$(includedir)/smooth

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) -r $(DESTDIR)$(includedir)/smooth-js
endif

	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(includedir)/smooth.h
endif

clean:
	$(call cleanin,classes)
	$(call cleanin,misc)
	$(call cleanin,resources)
	$(call cleanin,tools)
	$(call cleanin,samples)

	$(REMOVE) $(REMOVE_OPTS) $(DLLNAME) $(LIBNAME)

distclean: clean
	$(call cleanin,libraries)

	rmdir $(SMOOTH_PATH)/$(BINDIR) $(SMOOTH_PATH)/$(LIBDIR) || true

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(DLLNAME): objects libs
	$(LINKER) $(OBJECTS) $(LINKER_OPTS) $(LDFLAGS) $(LIBS)
ifeq ($(BUILD_WIN32),True)
	countbuild BuildNumber
endif
