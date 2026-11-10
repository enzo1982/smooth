########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_WAYLAND),True)
include $(SRCDIR)/Makefile-protocols

	OBJECTS += backendwayland.o $(addsuffix -protocol.o,$(WAYLAND_PROTOCOL_NAMES))

	MYCCOPTS += $(shell pkg-config --cflags wayland-client wayland-cursor xkbcommon libdecor-0)
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands

ifeq ($(BUILD_WAYLAND),True)
include $(SRCDIR)/Makefile-protocol-rules
endif
