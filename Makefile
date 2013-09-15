# Makefile for smooth v0.8

include Makefile-options
include Makefile-directories

### Output options ###

OBJECTS = classes/*/*.o classes/*/*/*.o classes/*/*/*/*.o classes/*/*/*/*/*.o misc/*.o

LIBS = -lfribidi -lbz2 -lxml2 -ljpeg -lz -lstdc++

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources/*.o

	ifeq ($(BUILD_X86),True)
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
	ifeq ($(BUILD_CAIRO),True)
		LIBS += -lcairo
	endif

	LIBS += -lcpuid -lnsucd -liconv -lpng -lpthread -lX11 -lXmu

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lpng -lbe -ltracker

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_QNX),True)
	LIBS += -liconv -lpng -lz -lsocket -lX11 -lXmu -lph

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else
	ifeq ($(BUILD_LINUX),True)
		LIBS += -lcpuid -lnsucd
	else ifeq ($(BUILD_FREEBSD),True)
		LIBS += -lrt
	else ifeq ($(BUILD_OPENBSD),True)
		LIBS += -lXau -lXdmcp -lXxf86vm -lSM -lICE -lffi -ldrm -lpcre
	endif

	LIBS += $(shell pkg-config --libs libpng) $(shell pkg-config --libs xmu) $(shell pkg-config --libs gtk+-2.0)
	LIBS += -lpthread

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
endif

### Linker options ###

LINKER	    = gcc
REMOVE	    = rm
LINKER_OPTS = -pipe -L$(LIBDIR) -o $(DLLNAME)
REMOVE_OPTS = -f

ifeq ($(BUILD_OSX),True)
ifeq ($(BUILD_X86),True)
	LINKER_OPTS += -dynamiclib -arch i386
endif
ifeq ($(BUILD_X86_64),True)
	LINKER_OPTS += -dynamiclib -arch x86_64
endif
ifeq ($(BUILD_PPC),True)
	LINKER_OPTS += -dynamiclib -arch ppc
endif
ifeq ($(BUILD_PPC64),True)
	LINKER_OPTS += -dynamiclib -arch ppc64
endif
else ifeq ($(BUILD_X86),True)
	LINKER_OPTS += --shared -m32 -s
else ifeq ($(BUILD_X86_64),True)
	LINKER_OPTS += --shared -m64 -s
endif

ifeq ($(BUILD_WIN32),True)
	LINKER_OPTS += -mwindows -Wl,--dynamicbase,--nxcompat,--kill-at,--out-implib,$(LIBNAME)
else ifeq ($(BUILD_OSX),True)
	LINKER_OPTS += -framework Cocoa -L/usr/X11/lib -Wl,-dylib_install_name,libsmooth-$(VERSION).$(REVISION)$(SHARED)
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

.PHONY: all objects programs libs install uninstall clean clean_all doc doc-clean

all: $(DLLNAME)

objects:
	$(MAKE) -C classes
	$(MAKE) -C misc
	$(MAKE) -C resources

programs: $(DLLNAME)
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

	$(COPY) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)

ifeq ($(BUILD_LINUX),True)
	ldconfig
else ifeq ($(BUILD_GNU),True)
	ldconfig
else ifeq ($(BUILD_FREEBSD),True)
	ldconfig
else ifeq ($(BUILD_NETBSD),True)
	ldconfig
else ifeq ($(BUILD_HAIKU),True)
	ldconfig
endif
else
	$(MKDIR) -p $(PREFIX)/$(LIB)
	$(COPY) $(DLLNAME) $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)

	$(COPY) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -s $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(CHMOD) a=r,u=rw $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
endif

	$(MKDIR) -p $(PREFIX)/bin
	$(COPY) $(BINDIR)/translator $(PREFIX)/bin
	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/bin/translator

	$(MKDIR) -p $(PREFIX)/include
	$(COPY) -r include/smooth $(PREFIX)/include
	$(COPY) -r include/smooth-js $(PREFIX)/include
	$(COPY) include/smooth.h $(PREFIX)/include

	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/include/smooth
	$(CHMOD) -R a=rX,u=rwX $(PREFIX)/include/smooth-js
	$(CHMOD) a=r,u=rw $(PREFIX)/include/smooth.h
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED).$(REVISION)

	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)

ifeq ($(BUILD_LINUX),True)
	ldconfig
else ifeq ($(BUILD_GNU),True)
	ldconfig
else ifeq ($(BUILD_FREEBSD),True)
	ldconfig
else ifeq ($(BUILD_NETBSD),True)
	ldconfig
else ifeq ($(BUILD_HAIKU),True)
	ldconfig
endif
else
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-$(VERSION).$(REVISION)$(SHARED)

	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/$(LIB)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
endif

	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/bin/translator

	$(REMOVE) $(REMOVE_OPTS) -r $(PREFIX)/include/smooth
	$(REMOVE) $(REMOVE_OPTS) -r $(PREFIX)/include/smooth-js
	$(REMOVE) $(REMOVE_OPTS) $(PREFIX)/include/smooth.h
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
