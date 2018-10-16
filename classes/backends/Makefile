########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = backend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cocoa) && $(call makein,gdiplus)
ALLCMD2   = $(call makein,haiku) && $(call makein,win32)
ALLCMD3   = $(call makein,xlib)
CLEANCMD1 = $(call cleanin,cocoa) && $(call cleanin,gdiplus)
CLEANCMD2 = $(call cleanin,haiku) && $(call cleanin,win32)
CLEANCMD3 = $(call cleanin,xlib)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
