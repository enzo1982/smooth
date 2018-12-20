########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBJPEG),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libjpeg
else ifeq ($(BUILD_NETBSD),True)
	MYCCOPTS += -I/usr/pkg/include
else
	MYCCOPTS += -I/usr/local/include
endif

ifeq ($(USE_BUNDLED_LIBPNG),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libpng
else
	MYCCOPTS += $(shell pkg-config --cflags libpng)
endif

# Enter object files here:
OBJECTS	  = imageloader.o jpeg.o pci.o png.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += icon.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
