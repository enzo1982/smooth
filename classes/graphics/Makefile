########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = bitmap.o color.o font.o surface.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,backends)
ALLCMD2   = $(call makein,forms)
ALLCMD3   = $(call makein,imageloader)
ALLCMD4   = $(call makein,modifiers)
CLEANCMD1 = $(call cleanin,backends)
CLEANCMD2 = $(call cleanin,forms)
CLEANCMD3 = $(call cleanin,imageloader)
CLEANCMD4 = $(call cleanin,modifiers)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
