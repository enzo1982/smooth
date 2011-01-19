########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = args.o binary.o config.o datetime.o math.o memory.o number.o string.o string_case.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd encoding && $(MAKE)
ALLCMD2   = cd hash && $(MAKE)
CLEANCMD1 = cd encoding && $(MAKE) clean
CLEANCMD2 = cd hash && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
