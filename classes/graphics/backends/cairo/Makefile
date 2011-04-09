########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifeq ($(BUILD_SOLARIS),True)
	MYCCOPTS += -I/usr/include/cairo
	MYCCOPTS += -I/usr/include/pango-1.0
	MYCCOPTS += -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
else
	MYCCOPTS += $(shell pkg-config --cflags pangocairo)
endif
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
