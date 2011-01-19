########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(BUILD_WIN32),True)
	MYCCOPTS = -I$(SMOOTH_PATH)/include/support/libjpeg -I$(SMOOTH_PATH)/include/support/libpng
endif

# Enter object files here:
OBJECTS	  = imageloader.o jpeg.o pci.o png.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += icon.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
