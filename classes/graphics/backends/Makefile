########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = bitmapbackend.o fontbackend.o surfacebackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd cairo && make
ALLCMD2   = cd gdi && make
ALLCMD3   = cd gdiplus && make
ALLCMD4   = cd xlib && make
CLEANCMD1 = cd cairo && make clean
CLEANCMD2 = cd gdi && make clean
CLEANCMD3 = cd gdiplus && make clean
CLEANCMD4 = cd xlib && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
