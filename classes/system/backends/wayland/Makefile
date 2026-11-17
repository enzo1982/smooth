########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_WAYLAND),True)
	OBJECTS += eventwayland.o screenwayland.o

	MYCCOPTS += $(shell $(PKG_CONFIG) --cflags wayland-client cairo)
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
