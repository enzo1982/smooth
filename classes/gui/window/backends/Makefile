########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

# Enter object files here:
OBJECTS	  = windowbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cocoa)
ALLCMD2   = $(call makein,gdi)
ALLCMD3   = $(call makein,haiku)
ALLCMD4   = $(call makein,xlib)
CLEANCMD1 = $(call cleanin,cocoa)
CLEANCMD2 = $(call cleanin,gdi)
CLEANCMD3 = $(call cleanin,haiku)
CLEANCMD4 = $(call cleanin,xlib)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
