########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_WIN32),True)
ifneq ($(BUILD_GDIPLUS),True)
	OBJECTS += bitmapgdi.o

ifneq ($(BUILD_CAIRO),True)
	OBJECTS += fontgdi.o surfacegdi.o
endif
endif
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
