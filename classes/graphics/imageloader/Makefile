########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libjpeg
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libpng
else ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libjpeg

	ifneq ($(BUILD_XLIB),True)
		MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libpng
	else
		MYCCOPTS += -I/usr/X11/include
	endif
else ifeq ($(BUILD_QNX),True)
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libjpeg
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/libpng
else ifeq ($(BUILD_NETBSD),True)
	MYCCOPTS += -I/usr/pkg/include $(shell pkg-config --cflags libpng)
else
	MYCCOPTS += -I/usr/local/include $(shell pkg-config --cflags libpng)
endif

# Enter object files here:
OBJECTS	  = imageloader.o jpeg.o pci.o png.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += icon.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
