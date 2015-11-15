########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBICONV),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libiconv
endif

# Enter object files here:
OBJECTS	  = 

ifeq ($(BUILD_WIN32),True)
	OBJECTS += backendwin32.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
