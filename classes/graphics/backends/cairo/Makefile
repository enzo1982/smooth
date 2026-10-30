########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	MYCCOPTS += $(shell $(PKG_CONFIG) --cflags pangocairo)
	MYCCOPTS += $(shell $(PKG_CONFIG) --cflags gdk-3.0)
endif
endif
else
ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support -DFRIBIDI_LIB_STATIC
endif
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_CAIRO),True)
	OBJECTS += fontcairo.o surfacecairo.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
