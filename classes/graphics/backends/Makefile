########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = bitmapbackend.o fontbackend.o surfacebackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd cairo && $(MAKE) && cd ../cocoa && $(MAKE)
ALLCMD2   = cd gdi && $(MAKE) && cd ../gdiplus && $(MAKE)
ALLCMD3   = cd haiku && $(MAKE) && cd ../xlib && $(MAKE)
CLEANCMD1 = cd cairo && $(MAKE) clean && cd ../cocoa && $(MAKE) clean
CLEANCMD2 = cd gdi && $(MAKE) clean && cd ../gdiplus && $(MAKE) clean
CLEANCMD3 = cd haiku && $(MAKE) clean && cd ../xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
