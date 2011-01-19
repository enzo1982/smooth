########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
	MYCCOPTS += -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/local/include/gtk-2.0 -I/usr/local/lib/gtk-2.0/include
	MYCCOPTS += -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include
	MYCCOPTS += -I/usr/include/gdk-pixbuf-2.0 -I/usr/local/include/gdk-pixbuf-2.0
	MYCCOPTS += -I/usr/include/atk-1.0 -I/usr/local/include/atk-1.0
	MYCCOPTS += -I/usr/include/cairo -I/usr/local/include/cairo
	MYCCOPTS += -I/usr/include/pango-1.0 -I/usr/local/include/pango-1.0
endif
endif

# Enter object files here:
OBJECTS	  = dirdlg.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += dirdlg_win32.o
else
ifeq ($(BUILD_OSX),True)
	OBJECTS += dirdlg_cocoa.o
else
	OBJECTS += dirdlg_gtk.o
endif
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
