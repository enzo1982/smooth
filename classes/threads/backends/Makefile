########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = mutexbackend.o semaphorebackend.o threadbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,cocoa)
ALLCMD2   = $(call makein,posix)
ALLCMD3   = $(call makein,win32)
CLEANCMD1 = $(call cleanin,cocoa)
CLEANCMD2 = $(call cleanin,posix)
CLEANCMD3 = $(call cleanin,win32)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
