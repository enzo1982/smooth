########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
	MYCCOPTS += -I/usr/local/include
endif

# Enter object files here:
OBJECTS	  = codecs.o init.o pciio.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
