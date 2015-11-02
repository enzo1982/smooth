########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = eventbackend.o screenbackend.o timerbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cocoa) && $(call makein,haiku)
ALLCMD2   = $(call makein,posix) && $(call makein,threads)
ALLCMD3   = $(call makein,win32) && $(call makein,xlib)
CLEANCMD1 = $(call cleanin,cocoa) && $(call cleanin,haiku)
CLEANCMD2 = $(call cleanin,posix) && $(call cleanin,threads)
CLEANCMD3 = $(call cleanin,win32) && $(call cleanin,xlib)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
