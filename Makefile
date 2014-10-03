# Makefile for smooth v0.8

include Makefile-options
include Makefile-directories

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

	LIBS += -lnsucd -lpng -lz -lws2_32 -limm32 -lole32

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

	LIBS += -lnsucd -liconv -lpthread

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lpng -lz -lbe -ltracker

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_QNX),True)
	LIBS += -liconv -lpng -lz -lsocket -lX11 -lXmu -lXft -lph

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else
	ifeq ($(BUILD_LINUX),True)
		LIBS += -lnsucd
	else ifeq ($(BUILD_FREEBSD),True)
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
LINKER_OPTS = -pipe -L$(LIBDIR) -o $(DLLNAME)
REMOVE_OPTS = -f

ifeq ($(BUILD_FREEBSD),True)
	LINKER = clang
endif

ifeq ($(BUILD_OSX),True)
	LINKER_OPTS += -dynamiclib -Wl,-x

ifeq ($(BUILD_X86),True)
	LINKER_OPTS += -arch i386
endif
ifeq ($(BUILD_X86_64),True)
	LINKER_OPTS += -arch x86_64
endif
ifeq ($(BUILD_PPC),True)
	LINKER_OPTS += -arch ppc
endif
ifeq ($(BUILD_PPC64),True)
	LINKER_OPTS += -arch ppc64
endif
else
	LINKER_OPTS += --shared -s

ifeq ($(BUILD_X86),True)
	LINKER_OPTS += -m32
else ifeq ($(BUILD_X86_64),True)
	LINKER_OPTS += -m64
endif
endif

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += -mwindows -Wl,--dynamicbase,--nxcompat,--kill-at,--out-implib,$(LIBNAME)
else ifeq ($(BUILD_OSX),True)
	LINKER_OPTS += -framework Cocoa -Wl,-dylib_install_name,libsmooth-$(VERSION).$(REVISION)$(SHARED)

	ifeq ($(BUILD_XLIB),True)
		LINKER_OPTS += -L/usr/X11/lib
	endif
else
	LINKER_OPTS += -Wl,-soname,libsmooth-$(VERSION)$(SHARED).$(REVISION)

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
MKDIR = mkdir

### Targets ###

.PHONY: all objects lib programs libs install uninstall clean clean_all doc doc-clean

all: lib programs

objects:
	$(MAKE) -C classes
	$(MAKE) -C misc
	$(MAKE) -C resources

lib: $(DLLNAME)

programs: lib
	$(MAKE) -C tools
	$(MAKE) -C samples

libs:
	$(MAKE) -C libraries

install: uninstall
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(MKDIR) -p $(PREFIX)/$(LIB)
	$(COPY) $(LIBDIR)/libsmooth-$(VERSION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(COPY) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
endif

ifeq ($(BUILD_LINUX),True)
	/sbin/ldconfig
else ifeq ($(BUILD_GNU),True)
	/sbin/ldconfig
else ifeq ($(BUILD_FREEBSD),True)
	/sbin/ldconfig
else ifeq ($(BUILD_NETBSD),True)
	/sbin/ldconfig
else ifeq ($(BUILD_OPENBSD),True)
	/sbin/ldconfig
endif
else
	$(MKDIR) -p $(PREFIX)/$(LIB)
	$(COPY) $(DLLNAME) $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(COPY) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
endif
endif

	$(MKDIR) -p $(PREFIX)/bin
	$(COPY) $(BINDIR)/translator $(PREFIX)/bin
	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/bin/translator

	$(MKDIR) -p $(PREFIX)/$(INCLUDE)
	$(COPY) -r include/smooth $(PREFIX)/$(INCLUDE)
	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/$(INCLUDE)/smooth

ifeq ($(BUILD_V8),True)
	$(COPY) -r include/smooth-js $(PREFIX)/$(INCLUDE)
	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/$(INCLUDE)/smooth-js
endif

	$(COPY) include/smooth.h $(PREFIX)/$(INCLUDE)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(INCLUDE)/smooth.h
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED).$(REVISION)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
endif

ifeq ($(BUILD_LINUX),True)
	/sbin/ldconfig
else ifeq ($(BUILD_GNU),True)
	/sbin/ldconfig
else ifeq ($(BUILD_FREEBSD),True)
	/sbin/ldconfig
else ifeq ($(BUILD_NETBSD),True)
	/sbin/ldconfig
else ifeq ($(BUILD_OPENBSD),True)
	/sbin/ldconfig
endif
else
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
endif
endif

	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/bin/translator

	$(REMOVE) $(REMOVE_OPTS) -r $(PREFIX)/$(INCLUDE)/smooth

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) -r $(PREFIX)/$(INCLUDE)/smooth-js
endif

	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(INCLUDE)/smooth.h
endif

clean:
	$(MAKE) -C classes clean
	$(MAKE) -C misc clean
	$(MAKE) -C resources clean
	$(MAKE) -C tools clean
	$(MAKE) -C samples clean
	$(REMOVE) $(REMOVE_OPTS) $(DLLNAME) $(LIBNAME)

clean_all: clean
	$(MAKE) -C libraries clean

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(DLLNAME): objects libs
	$(LINKER) $(OBJECTS) $(LINKER_OPTS) $(LIBS)
ifeq ($(BUILD_WIN32),True)
	countbuild BuildNumber
endif
