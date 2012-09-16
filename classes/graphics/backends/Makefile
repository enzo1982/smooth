########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = bitmapbackend.o fontbackend.o surfacebackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd cairo && $(MAKE)
ALLCMD2   = cd gdi && $(MAKE)
ALLCMD3   = cd gdiplus && $(MAKE)
ALLCMD4   = cd haiku && $(MAKE)
ALLCMD5   = cd xlib && $(MAKE)
CLEANCMD1 = cd cairo && $(MAKE) clean
CLEANCMD2 = cd gdi && $(MAKE) clean
CLEANCMD3 = cd gdiplus && $(MAKE) clean
CLEANCMD4 = cd haiku && $(MAKE) clean
CLEANCMD5 = cd xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
