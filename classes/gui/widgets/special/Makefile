########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support
endif

# Enter object files here:
OBJECTS	  = cursor.o dragcontrol.o droparea.o shortcut.o tooltip.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
