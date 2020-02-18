########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBXML2),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libxml2
else ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I$(shell xcodebuild -sdk macosx -version | grep "^Path: " | head -n 1 | tail -c +7)/usr/include/libxml2
else
	MYCCOPTS += $(shell pkg-config --cflags libxml-2.0)
endif

ifeq ($(USE_BUNDLED_LIBICONV),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libiconv
else
	MYCCOPTS += -I/usr/local/include
endif

# Enter object files here:
OBJECTS	  = attribute.o document.o node.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,xul)
CLEANCMD1 = $(call cleanin,xul)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
