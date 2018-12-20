########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	MYCCOPTS += $(shell pkg-config --cflags x11)
endif
endif
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_XLIB),True)
	OBJECTS += clipboardxlib.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
