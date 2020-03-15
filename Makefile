# Makefile for smooth v0.9

SMOOTH_PATH = .

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

### Object files ###

OBJECTS  = classes/backends/backend.o
OBJECTS += classes/basic/object.o classes/basic/objecttype.o classes/basic/setup.o
OBJECTS += classes/errors/error.o classes/errors/success.o
OBJECTS += classes/errors/fs/endoffile.o classes/errors/fs/filenotfound.o
OBJECTS += classes/errors/misc/permissiondenied.o
OBJECTS += classes/files/directory.o classes/files/file.o
OBJECTS += classes/graphics/bitmap.o classes/graphics/color.o classes/graphics/font.o classes/graphics/surface.o
OBJECTS += classes/graphics/backends/bitmapbackend.o classes/graphics/backends/fontbackend.o classes/graphics/backends/surfacebackend.o
OBJECTS += classes/graphics/forms/rect.o
OBJECTS += classes/graphics/imageloader/imageloader.o classes/graphics/imageloader/jpeg.o classes/graphics/imageloader/pci.o classes/graphics/imageloader/png.o
OBJECTS += classes/graphics/modifiers/fontsize.o classes/graphics/modifiers/righttoleft.o
OBJECTS += classes/gui/application/application.o
OBJECTS += classes/gui/clipboard/clipboard.o
OBJECTS += classes/gui/clipboard/backends/clipboardbackend.o
OBJECTS += classes/gui/dialogs/colordlg.o classes/gui/dialogs/dialog.o classes/gui/dialogs/fontdlg.o classes/gui/dialogs/messagebox.o classes/gui/dialogs/splashscreen.o classes/gui/dialogs/tipodaydlg.o
OBJECTS += classes/gui/dialogs/directory/dirdlg.o
OBJECTS += classes/gui/dialogs/file/filedlg.o
OBJECTS += classes/gui/dialogs/icons/light.o
OBJECTS += classes/gui/widgets/layer.o classes/gui/widgets/widget.o
OBJECTS += classes/gui/widgets/basic/activearea.o classes/gui/widgets/basic/arrows.o classes/gui/widgets/basic/button.o classes/gui/widgets/basic/checkbox.o classes/gui/widgets/basic/client.o classes/gui/widgets/basic/divider.o classes/gui/widgets/basic/editbox.o classes/gui/widgets/basic/groupbox.o classes/gui/widgets/basic/hyperlink.o classes/gui/widgets/basic/image.o classes/gui/widgets/basic/multiedit.o classes/gui/widgets/basic/optionbox.o classes/gui/widgets/basic/progressbar.o classes/gui/widgets/basic/scrollbar.o classes/gui/widgets/basic/slider.o classes/gui/widgets/basic/statusbar.o classes/gui/widgets/basic/tabwidget.o classes/gui/widgets/basic/text.o classes/gui/widgets/basic/titlebar.o
OBJECTS += classes/gui/widgets/hotspot/hotspot.o classes/gui/widgets/hotspot/simplebutton.o
OBJECTS += classes/gui/widgets/multi/generic/container.o classes/gui/widgets/multi/generic/entry.o
OBJECTS += classes/gui/widgets/multi/image/imagebox.o classes/gui/widgets/multi/image/imageentry.o
OBJECTS += classes/gui/widgets/multi/list/combobox.o classes/gui/widgets/multi/list/list.o classes/gui/widgets/multi/list/listbox.o classes/gui/widgets/multi/list/listboxheader.o classes/gui/widgets/multi/list/listentry.o classes/gui/widgets/multi/list/listentryseparator.o
OBJECTS += classes/gui/widgets/multi/menu/menu.o classes/gui/widgets/multi/menu/menubar.o classes/gui/widgets/multi/menu/menubarentry.o classes/gui/widgets/multi/menu/menuentry.o classes/gui/widgets/multi/menu/micromenu.o classes/gui/widgets/multi/menu/popupmenu.o classes/gui/widgets/multi/menu/popupmenuentry.o classes/gui/widgets/multi/menu/popupmenuentrycheck.o classes/gui/widgets/multi/menu/popupmenuentryoption.o
OBJECTS += classes/gui/widgets/multi/tree/tree.o
OBJECTS += classes/gui/widgets/special/cursor.o classes/gui/widgets/special/dragcontrol.o classes/gui/widgets/special/droparea.o classes/gui/widgets/special/shortcut.o classes/gui/widgets/special/tooltip.o
OBJECTS += classes/gui/window/toolwindow.o classes/gui/window/window.o
OBJECTS += classes/gui/window/backends/windowbackend.o
OBJECTS += classes/i18n/language.o classes/i18n/number.o classes/i18n/section.o classes/i18n/translator.o classes/i18n/translator_internal.o
OBJECTS += classes/input/keyboard.o classes/input/pointer.o
OBJECTS += classes/input/backends/pointerbackend.o
OBJECTS += classes/io/driver.o classes/io/filter.o classes/io/instream.o classes/io/outstream.o classes/io/stream.o
OBJECTS += classes/io/drivers/driver_ansi.o classes/io/drivers/driver_https.o classes/io/drivers/driver_memory.o classes/io/drivers/driver_posix.o classes/io/drivers/driver_socket.o classes/io/drivers/driver_socks4.o classes/io/drivers/driver_socks5.o classes/io/drivers/driver_zero.o
OBJECTS += classes/io/filters/filter_bzip2.o classes/io/filters/filter_xor.o
OBJECTS += classes/misc/args.o classes/misc/array.o classes/misc/binary.o classes/misc/config.o classes/misc/datetime.o classes/misc/math.o classes/misc/memory.o classes/misc/number.o classes/misc/string.o classes/misc/string_case.o
OBJECTS += classes/misc/encoding/base64.o classes/misc/encoding/urlencode.o
OBJECTS += classes/misc/hash/crc16.o classes/misc/hash/crc32.o classes/misc/hash/crc64.o classes/misc/hash/md5.o classes/misc/hash/sha1.o
OBJECTS += classes/net/protocols/file.o classes/net/protocols/http.o classes/net/protocols/protocol.o
OBJECTS += classes/system/console.o classes/system/cpu.o classes/system/dynamicloader.o classes/system/event.o classes/system/screen.o classes/system/system.o classes/system/timer.o
OBJECTS += classes/system/backends/eventbackend.o classes/system/backends/screenbackend.o classes/system/backends/timerbackend.o
OBJECTS += classes/threads/access.o classes/threads/mutex.o classes/threads/rwlock.o classes/threads/semaphore.o classes/threads/thread.o
OBJECTS += classes/threads/backends/mutexbackend.o classes/threads/backends/semaphorebackend.o classes/threads/backends/threadbackend.o
OBJECTS += classes/xml/attribute.o classes/xml/document.o classes/xml/node.o
OBJECTS += classes/xml/xul/box.o classes/xml/xul/button.o classes/xml/xul/description.o classes/xml/xul/label.o classes/xml/xul/menubar.o classes/xml/xul/popupmenu.o classes/xml/xul/renderer.o classes/xml/xul/textbox.o classes/xml/xul/widget.o classes/xml/xul/window.o
OBJECTS += misc/codecs.o misc/init.o misc/pciio.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += classes/backends/win32/backendwin32.o
	OBJECTS += classes/graphics/imageloader/icon.o
	OBJECTS += classes/gui/application/xulloader.o
	OBJECTS += classes/gui/clipboard/backends/win32/clipboardwin32.o
	OBJECTS += classes/gui/dialogs/directory/dirdlg_win32.o
	OBJECTS += classes/gui/dialogs/file/filedlg_win32.o
	OBJECTS += classes/gui/window/backends/gdi/droptarget.o classes/gui/window/backends/gdi/windowgdi.o
	OBJECTS += classes/input/backends/win32/pointerwin32.o
	OBJECTS += classes/io/drivers/driver_win32.o
	OBJECTS += classes/system/backends/win32/eventwin32.o classes/system/backends/win32/screenwin32.o classes/system/backends/win32/timerwin32.o
	OBJECTS += classes/threads/backends/win32/mutexwin32.o classes/threads/backends/win32/semaphorewin32.o classes/threads/backends/win32/threadwin32.o

	ifeq ($(BUILD_GDIPLUS),True)
		OBJECTS += classes/backends/gdiplus/backendgdiplus.o
		OBJECTS += classes/graphics/backends/gdiplus/bitmapgdiplus.o classes/graphics/backends/gdiplus/fontgdiplus.o classes/graphics/backends/gdiplus/surfacegdiplus.o
	else
		OBJECTS += classes/graphics/backends/gdi/bitmapgdi.o classes/graphics/backends/gdi/fontgdi.o classes/graphics/backends/gdi/surfacegdi.o
	endif
else ifeq ($(BUILD_OSX),True)
	OBJECTS += classes/backends/cocoa/backendcocoa.o
	OBJECTS += classes/graphics/backends/cocoa/bitmapcocoa.o classes/graphics/backends/cocoa/fontcocoa.o classes/graphics/backends/cocoa/surfacecocoa.o
	OBJECTS += classes/gui/clipboard/backends/cocoa/clipboardcocoa.o
	OBJECTS += classes/gui/dialogs/directory/dirdlg_cocoa.o
	OBJECTS += classes/gui/dialogs/file/filedlg_cocoa.o
	OBJECTS += classes/gui/widgets/multi/menu/menubar_cocoa.o
	OBJECTS += classes/gui/window/backends/cocoa/windowcocoa.o
	OBJECTS += classes/input/backends/cocoa/pointercocoa.o
	OBJECTS += classes/system/backends/cocoa/eventcocoa.o classes/system/backends/cocoa/screencocoa.o classes/system/backends/cocoa/timercocoa.o
else ifeq ($(BUILD_HAIKU),True)
	OBJECTS += classes/backends/haiku/backendhaiku.o 
	OBJECTS += classes/graphics/backends/haiku/bitmaphaiku.o classes/graphics/backends/haiku/fonthaiku.o classes/graphics/backends/haiku/surfacehaiku.o 
	OBJECTS += classes/gui/clipboard/backends/haiku/clipboardhaiku.o
	OBJECTS += classes/gui/dialogs/directory/dirdlg_haiku.o
	OBJECTS += classes/gui/dialogs/file/filedlg_haiku.o
	OBJECTS += classes/gui/window/backends/haiku/windowhaiku.o
	OBJECTS += classes/input/backends/haiku/pointerhaiku.o
	OBJECTS += classes/system/backends/haiku/eventhaiku.o classes/system/backends/haiku/screenhaiku.o classes/system/backends/haiku/timerhaiku.o
else
	OBJECTS += classes/gui/dialogs/directory/dirdlg_gtk.o
	OBJECTS += classes/gui/dialogs/file/filedlg_gtk.o
endif

ifeq ($(BUILD_XLIB),True)
	OBJECTS += classes/backends/xlib/backendxlib.o
	OBJECTS += classes/graphics/backends/xlib/bitmapxlib.o
	OBJECTS += classes/gui/clipboard/backends/xlib/clipboardxlib.o
	OBJECTS += classes/gui/window/backends/xlib/windowxlib.o
	OBJECTS += classes/input/backends/xlib/pointerxlib.o
	OBJECTS += classes/system/backends/xlib/eventxlib.o classes/system/backends/xlib/screenxlib.o

	ifeq ($(BUILD_CAIRO),True)
		OBJECTS += classes/graphics/backends/cairo/fontcairo.o classes/graphics/backends/cairo/surfacecairo.o
	else
		OBJECTS += classes/graphics/backends/xlib/fontxlib.o classes/graphics/backends/xlib/surfacexlib.o
	endif
endif

ifeq ($(BUILD_POSIXTIMER),True)
	OBJECTS += classes/system/backends/posix/timerposix.o
else ifeq ($(BUILD_THREADSTIMER),True)
	OBJECTS += classes/system/backends/threads/timerthreads.o
endif 

ifeq ($(BUILD_POSIXTHREADS),True)
	OBJECTS += classes/threads/backends/posix/mutexposix.o classes/threads/backends/posix/semaphoreposix.o

	ifeq ($(BUILD_OSX),True)
		OBJECTS += classes/threads/backends/cocoa/threadcocoa.o
	else
		OBJECTS += classes/threads/backends/posix/threadposix.o
	endif
endif 

### Output options ###

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

ifeq ($(USE_BUNDLED_LIBCURL),True)
	LIBS += $(LIBDIR)/libcurl.a

	ifeq ($(BUILD_WIN32),True)
		LIBS += -lwldap32 -lcrypt32
	else ifeq ($(BUILD_OSX),True)
		LIBS += -framework Security -lldap
	endif
else ifeq ($(BUILD_OSX),True)
	LIBS += -lcurl
else
	LIBS += $(shell pkg-config --libs libcurl)
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
	OBJECTS += resources/resources.o

	ifeq ($(BUILD_GDIPLUS),True)
		LIBS += -lgdiplus
	endif

	LIBS += -lws2_32 -limm32 -lole32 -luuid

	DLLNAME = $(BINDIR)/smooth$(SHARED)
	LIBNAME = $(LIBDIR)/libsmooth.a
else ifeq ($(BUILD_OSX),True)
	LIBS += -lpthread

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else ifeq ($(BUILD_HAIKU),True)
	LIBS += -lbe -ltracker -lnetwork

	DLLNAME = $(LIBDIR)/libsmooth-$(VERSION)$(SHARED)
else
	ifeq ($(BUILD_FREEBSD),True)
		LIBS += -lrt
	else ifeq ($(BUILD_LINUX),True)
		LIBS += -lrt
	else ifeq ($(BUILD_OPENBSD),True)
		LIBS += -lXau -lXdmcp -lXxf86vm -lSM -lICE -lffi -ldrm -lpcre
	endif

	ifneq ($(BUILD_CAIRO),True)
		LIBS += $(shell pkg-config --libs xft)
	endif

	LIBS += -lpthread
	LIBS += $(shell pkg-config --libs gtk+-3.0)

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
else
	LINKER_OPTS += --shared -Wl,-soname,libsmooth-$(VERSION)$(SHARED).$(REVISION)

	ifeq ($(BUILD_FREEBSD),True)
		LINKER_OPTS += -L/usr/local/lib
	else ifeq ($(BUILD_OPENBSD),True)
		LINKER_OPTS += -Wl,-rpath,/usr/local/lib -L/usr/local/lib -Wl,-rpath,/usr/X11R6/lib -L/usr/X11R6/lib
	else ifeq ($(BUILD_NETBSD),True)
		LINKER_OPTS += -Wl,-rpath,/usr/pkg/lib -L/usr/pkg/lib -Wl,-rpath,/usr/X11R7/lib -L/usr/X11R7/lib
	endif
endif

### Install options ###
COPY  = cp
LINK  = ln
CHMOD = chmod

### Targets ###

.PHONY: all codesign folders objects lib programs libs install uninstall clean distclean doc doc-clean

all: lib programs

folders:
	mkdir -p $(SMOOTH_PATH)/$(BINDIR) $(SMOOTH_PATH)/$(LIBDIR)

objects:
	+ $(call makein,classes)
	+ $(call makein,misc)
	+ $(call makein,resources)

lib: folders $(DLLNAME)

programs: lib
	+ $(call makein,tools)
	+ $(call makein,samples)

libs:
	+ $(call makein,libraries)

codesign: all
	signtool sign -fd sha1 -tr http://timestamp.digicert.com -td sha1 $(BINDIR)/smooth*.dll $(BINDIR)/translator.exe
	signtool sign -fd sha256 -tr http://timestamp.digicert.com -td sha256 -as $(BINDIR)/smooth*.dll $(BINDIR)/translator.exe

install: all
ifneq ($(BUILD_WIN32),True)
	$(INSTALL) -d "$(DESTDIR)"$(libdir)

ifneq ($(BUILD_OSX),True)
	$(INSTALL_DATA) $(DLLNAME) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-$(VERSION)$(SHARED).$(REVISION) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
	$(LINK) -fs libsmooth-js-$(VERSION)$(SHARED).$(REVISION) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif

ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_HAIKU),True)
ifeq ("$(DESTDIR)","")
	$(LDCONFIG) 2> /dev/null || true
endif
endif
endif
else
	$(INSTALL_DATA) $(DLLNAME) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-$(VERSION).$(REVISION)$(SHARED) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(INSTALL_DATA) $(LIBDIR)/libsmooth-js-$(VERSION)$(SHARED) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
	$(LINK) -fs libsmooth-js-$(VERSION).$(REVISION)$(SHARED) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
endif
endif

	$(INSTALL) -d "$(DESTDIR)"$(bindir)
	$(INSTALL_PROGRAM) $(BINDIR)/translator "$(DESTDIR)"$(bindir)/smooth-translator

	$(INSTALL) -d "$(DESTDIR)"$(includedir)
	$(COPY) -r "$(SRCDIR)"/include/smooth "$(DESTDIR)"$(includedir)
	$(CHMOD) -R a=rX,u=rwX "$(DESTDIR)"$(includedir)/smooth

ifeq ($(BUILD_V8),True)
	$(COPY) -r "$(SRCDIR)"/include/smooth-js "$(DESTDIR)"$(includedir)
	$(CHMOD) -R a=rX,u=rwX "$(DESTDIR)"$(includedir)/smooth-js
endif

	$(INSTALL_DATA) "$(SRCDIR)"/include/smooth.h "$(DESTDIR)"$(includedir)
endif

uninstall:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED).$(REVISION)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED).$(REVISION)
endif

ifneq ($(BUILD_SOLARIS),True)
ifneq ($(BUILD_HAIKU),True)
ifeq ("$(DESTDIR)","")
	$(LDCONFIG) 2> /dev/null || true
endif
endif
endif
else
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-$(VERSION).$(REVISION)$(SHARED)

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION)$(SHARED)
	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(libdir)/libsmooth-js-$(VERSION).$(REVISION)$(SHARED)
endif
endif

	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(bindir)/smooth-translator

	$(REMOVE) $(REMOVE_OPTS) -r "$(DESTDIR)"$(includedir)/smooth

ifeq ($(BUILD_V8),True)
	$(REMOVE) $(REMOVE_OPTS) -r "$(DESTDIR)"$(includedir)/smooth-js
endif

	$(REMOVE) $(REMOVE_OPTS) "$(DESTDIR)"$(includedir)/smooth.h
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

ifneq ("$(SRCDIR)","$(CURDIR)")
	rmdir $(SMOOTH_PATH)/$(BINDIR) $(SMOOTH_PATH)/$(LIBDIR) || true
endif

doc: doc-clean
	doxys

doc-clean:
	rm -r -f doc/reference

$(OBJECTS): objects
	

$(DLLNAME): $(OBJECTS) | libs
	$(LD) $(OBJECTS) $(LINKER_OPTS) $(LDFLAGS) $(LIBS)
ifeq ($(findstring release,$(config)),release)
	-countbuild BuildNumber
endif
