########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(SMOOTH_PATH)/Makefile-options

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
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_CAIRO),True)
	OBJECTS += fontcairo.o surfacecairo.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
