########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = backend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd gdiplus && make
ALLCMD2   = cd win32 && make
ALLCMD3   = cd xlib && make
CLEANCMD1 = cd gdiplus && make clean
CLEANCMD2 = cd win32 && make clean
CLEANCMD3 = cd xlib && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
