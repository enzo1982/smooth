########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = backend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cocoa)
ALLCMD2   = $(call makein,gdiplus)
ALLCMD3   = $(call makein,win32)
ALLCMD4   = $(call makein,xlib)
CLEANCMD1 = $(call cleanin,cocoa)
CLEANCMD2 = $(call cleanin,gdiplus)
CLEANCMD3 = $(call cleanin,win32)
CLEANCMD4 = $(call cleanin,xlib)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
