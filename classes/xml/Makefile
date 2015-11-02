########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I/usr/include/libxml2
else ifeq ($(BUILD_HAIKU),True)
	MYCCOPTS += -I/system/develop/headers/libxml2
else ifneq ($(BUILD_QNX),True)
	MYCCOPTS += $(shell pkg-config --cflags libxml-2.0)
endif
endif

# Enter object files here:
OBJECTS	  = attribute.o document.o node.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,xul)
CLEANCMD1 = $(call cleanin,xul)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
