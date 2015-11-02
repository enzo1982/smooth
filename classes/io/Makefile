########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = driver.o filter.o instream.o outstream.o stream.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,drivers)
ALLCMD2   = $(call makein,filters)
CLEANCMD1 = $(call cleanin,drivers)
CLEANCMD2 = $(call cleanin,filters)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
