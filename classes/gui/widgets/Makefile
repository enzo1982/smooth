########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = layer.o widget.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd basic && $(MAKE)
ALLCMD2   = cd hotspot && $(MAKE)
ALLCMD3   = cd multi && $(MAKE)
ALLCMD4   = cd special && $(MAKE)
CLEANCMD1 = cd basic && $(MAKE) clean
CLEANCMD2 = cd hotspot && $(MAKE) clean
CLEANCMD3 = cd multi && $(MAKE) clean
CLEANCMD4 = cd special && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
