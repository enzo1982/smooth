########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = bitmap.o color.o font.o surface.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd backends && $(MAKE)
ALLCMD2   = cd forms && $(MAKE)
ALLCMD3   = cd imageloader && $(MAKE)
ALLCMD4   = cd modifiers && $(MAKE)
CLEANCMD1 = cd backends && $(MAKE) clean
CLEANCMD2 = cd forms && $(MAKE) clean
CLEANCMD3 = cd imageloader && $(MAKE) clean
CLEANCMD4 = cd modifiers && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
