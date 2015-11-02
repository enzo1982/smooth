########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = layer.o widget.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,basic)
ALLCMD2   = $(call makein,hotspot)
ALLCMD3   = $(call makein,multi)
ALLCMD4   = $(call makein,special)
CLEANCMD1 = $(call cleanin,basic)
CLEANCMD2 = $(call cleanin,hotspot)
CLEANCMD3 = $(call cleanin,multi)
CLEANCMD4 = $(call cleanin,special)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
