########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBPNG),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libpng
else
	MYCCOPTS += $(shell $(PKG_CONFIG) --cflags libpng)
endif

# Enter object files here:
OBJECTS	  = json.o server.o tools.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
