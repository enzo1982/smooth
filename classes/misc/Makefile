########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
	MYCCOPTS += -I/usr/local/include
endif

# Enter object files here:
OBJECTS	  = args.o array.o binary.o config.o datetime.o math.o memory.o number.o string.o string_case.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd encoding && $(MAKE)
ALLCMD2   = cd hash && $(MAKE)
CLEANCMD1 = cd encoding && $(MAKE) clean
CLEANCMD2 = cd hash && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
