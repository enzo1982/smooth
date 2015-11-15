# Makefile for smooth v0.8

SMOOTH_PATH = .

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

### Output options ###

OBJECTS = classes/*/*.o classes/*/*/*.o classes/*/*/*/*.o classes/*/*/*/*/*.o misc/*.o

LIBS = -lstdc++

ifeq ($(USE_BUNDLED_LIBBZ2),True)
	LIBS += $(LIBDIR)/libbz2.a
else
	LIBS += -lbz2
endif

ifeq ($(USE_BUNDLED_LIBCPUID),True)
	LIBS += $(LIBDIR)/libcpuid.a
else
	LIBS += -lcpuid
endif

ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	LIBS += $(LIBDIR)/libfribidi.a
else
	LIBS += -lfribidi
endif

ifeq ($(USE_BUNDLED_LIBICONV),True)
	LIBS += $(LIBDIR)/libiconv.a
else ifeq ($(BUILD_OSX),True)
	LIBS += -liconv
else ifeq ($(BUILD_QNX),True)
	LIBS += -liconv
endif

ifeq ($(USE_BUNDLED_LIBJPEG),True)
	LIBS += $(LIBDIR)/libjpeg.a
else
	LIBS += -ljpeg
endif

ifeq ($(USE_BUNDLED_LIBPNG),True)
	LIBS += $(LIBDIR)/libpng.a
else ifeq ($(BUILD_WIN32),True)
	LIBS += -lpng
else ifeq ($(BUILD_OSX),True)
	LIBS += -lpng
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lpng
else ifeq ($(BUILD_QNX),True)
	LIBS += -lpng
else
	LIBS += $(shell pkg-config --libs libpng)
endif

ifeq ($(USE_BUNDLED_LIBXML2),True)
	LIBS += $(LIBDIR)/libxml2.a
else
	LIBS += -lxml2
endif

ifeq ($(USE_BUNDLED_ZLIB),True)
	LIBS += $(LIBDIR)/libz.a
else
	LIBS += -lz
endif

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources/*.o

	ifeq ($(BUILD_GDIPLUS),True)
		LIBS += -lgdiplus
	endif

	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo.dll
	endif

	LIBS += -lws2_32 -limm32 -lole32 -luuid

	DLLNAME = $(BINDIR)/smooth$(SHARED)
	LIBNAME = $(LIBDIR)/libsmooth.a
else ifeq ($(BUILD_OSX),True)
	ifeq ($(BUILD_XLIB),True)
		LIBS += -lX11 -lXmu -lXft
	endif

	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo
	endif

	LIBS += -lpthread

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lbe -ltracker

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_QNX),True)
	LIBS += -lsocket -lX11 -lXmu -lXft -lph

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else
	ifeq ($(BUILD_FREEBSD),True)
		LIBS += -lrt
	else ifeq ($(BUILD_OPENBSD),True)
		LIBS += -lXau -lXdmcp -lXxf86vm -lSM -lICE -lffi -ldrm -lpcre
	endif

	LIBS += $(shell pkg-config --libs xmu) $(shell pkg-config --libs xft)
	LIBS += -lpthread

	ifeq ($(BUILD_SOLARIS),True)
		LIBS += $(shell pkg-config --libs gtk+-2.0)
	else
		LIBS += $(shell pkg-config --libs gtk+-3.0)
	endif

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
endif

### Linker options ###

REMOVE	    = rm
LINKER_OPTS = -L$(LIBDIR) -o $(DLLNAME)
REMOVE_OPTS = -f

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
	+ $(call makein,classes)
	+ $(call makein,misc)
	+ $(call makein,resources)

lib: $(DLLNAME)

programs: lib
	+ $(call makein,tools)
	+ $(call makein,samples)

libs:
	+ $(call makein,libraries)

install: all
ifneq ($(BUILD_WIN32),True)
	$(INSTALL) -d $(DESTDIR)$(libdir)

ifneq ($(BUILD_OSX),True)
	$(INSTALL_DATA) $(DLLNAME) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-$(VERSION)$(SHARED).$(REVISION) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif

ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_HAIKU),True)
ifneq ($(BUILD_QNX),True)
ifeq ($(DESTDIR),)
	$(LDCONFIG) 2> /dev/null || true
endif
endif
endif
endif
else
	$(INSTALL_DATA) $(DLLNAME) $(DESTDIR)$(libdir)/libsmooth-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-$(VERSION).$(REVISION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(DESTDIR)$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif
endif

	$(INSTALL) -d $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) $(BINDIR)/translator $(DESTDIR)$(bindir)/smooth-translator

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
ifeq ($(DESTDIR),)
	$(LDCONFIG) 2> /dev/null || true
endif
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

	$(REMOVE) $(REMOVE_OPTS) $(DESTDIR)$(bindir)/smooth-translator

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

ifneq ($(SRCDIR),$(CURDIR))
	rmdir $(SMOOTH_PATH)/$(BINDIR) $(SMOOTH_PATH)/$(LIBDIR) || true
endif

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(DLLNAME): objects libs
	$(LD) $(OBJECTS) $(LINKER_OPTS) $(LDFLAGS) $(LIBS)
ifeq ($(BUILD_WIN32),True)
	countbuild BuildNumber
endif
