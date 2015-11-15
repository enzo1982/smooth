########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_HAIKU),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I/usr/X11/include
else ifeq ($(BUILD_QNX),True)
	MYCCOPTS += -I/usr/X11R6/include
else
	MYCCOPTS += $(shell pkg-config --cflags pangocairo)
endif
endif
else
ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support
endif
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_CAIRO),True)
	OBJECTS += fontcairo.o surfacecairo.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
