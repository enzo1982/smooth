########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALEXE = translator

# Enter object files here:
OBJECTS	  = infoitem.o section.o stringitem.o translator.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += resources.o
endif

# Enter addition commands for targets all and clean here:
ALLCMD1   =
ALLCMD2   =
ALLCMD3   =
CLEANCMD1 =
CLEANCMD2 =
CLEANCMD3 =

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
