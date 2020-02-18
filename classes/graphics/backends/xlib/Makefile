########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	MYCCOPTS += $(shell pkg-config --cflags x11)

ifneq ($(BUILD_CAIRO),True)
	MYCCOPTS += $(shell pkg-config --cflags xft)
endif
endif
endif
endif

ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_XLIB),True)
	OBJECTS += bitmapxlib.o

ifneq ($(BUILD_CAIRO),True)
	OBJECTS += fontxlib.o surfacexlib.o
endif
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
