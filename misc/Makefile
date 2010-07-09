########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

# Enter object files here:
OBJECTS	  = codecs.o init.o pciio.o

ifeq ($(EXPLICIT_TEMPLATES),True)
	OBJECTS += templates.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
