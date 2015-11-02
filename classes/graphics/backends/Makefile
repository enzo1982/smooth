########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = bitmapbackend.o fontbackend.o surfacebackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cairo) && $(call makein,cocoa)
ALLCMD2   = $(call makein,gdi) && $(call makein,gdiplus)
ALLCMD3   = $(call makein,haiku) && $(call makein,xlib)
CLEANCMD1 = $(call cleanin,cairo) && $(call cleanin,cocoa)
CLEANCMD2 = $(call cleanin,gdi) && $(call cleanin,gdiplus)
CLEANCMD3 = $(call cleanin,haiku) && $(call cleanin,xlib)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
