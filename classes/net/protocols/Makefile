########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBCURL),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support -DCURL_STATICLIB
else ifneq ($(BUILD_OSX),True)
	MYCCOPTS += $(shell pkg-config --cflags libcurl)
endif

# Enter object files here:
OBJECTS	  = file.o http.o protocol.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
