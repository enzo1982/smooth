########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = base64.o urlencode.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands

# Optimize encoding functions for speed in release mode:
ifeq ($(findstring release,$(config)),release)
	override CXXFLAGS += -O3
endif
