########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -I$(SMOOTH_PATH)/include/support/bzlib
endif

# Enter object files here:
OBJECTS	  = filter_bzip2.o filter_xor.o

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
