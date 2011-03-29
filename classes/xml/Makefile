########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
	MYCCOPTS += -I/usr/include/libxml2 -I/usr/local/include/libxml2
endif

# Enter object files here:
OBJECTS	  = attribute.o document.o node.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd xul && $(MAKE)
CLEANCMD1 = cd xul && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
