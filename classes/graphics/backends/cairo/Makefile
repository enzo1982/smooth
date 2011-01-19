########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I/opt/local/include/cairo
	MYCCOPTS += -I/opt/local/include/pango-1.0
	MYCCOPTS += -I/opt/local/include/glib-2.0 -I/opt/local/lib/glib-2.0/include
else
	MYCCOPTS += -I/usr/include/cairo -I/usr/local/include/cairo
	MYCCOPTS += -I/usr/include/pango-1.0 -I/usr/local/include/pango-1.0
	MYCCOPTS += -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include
endif
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_CAIRO),True)
	OBJECTS += fontcairo.o surfacecairo.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
