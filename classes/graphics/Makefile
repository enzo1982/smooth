########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = bitmap.o color.o font.o surface.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd backends && make
ALLCMD2   = cd forms && make
ALLCMD3   = cd imageloader && make
ALLCMD4   = cd modifiers && make
CLEANCMD1 = cd backends && make clean
CLEANCMD2 = cd forms && make clean
CLEANCMD3 = cd imageloader && make clean
CLEANCMD4 = cd modifiers && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
