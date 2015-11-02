########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = error.o success.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,fs)
ALLCMD2   = $(call makein,misc)
CLEANCMD1 = $(call cleanin,fs)
CLEANCMD2 = $(call cleanin,misc)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
