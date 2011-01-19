########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

# Enter object files here:
OBJECTS	  = windowbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd gdi && $(MAKE)
ALLCMD2   = cd xlib && $(MAKE)
CLEANCMD1 = cd gdi && $(MAKE) clean
CLEANCMD2 = cd xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
