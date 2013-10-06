########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Disable PowerPC builds of V8 on OS X for now.
ifeq ($(BUILD_OSX),True)
	BUILD_PPC   = False
	BUILD_PPC64 = False
endif

# Change these variables to fit your project:
LOCALEXE  = v8test

# Enter object files here:
OBJECTS	  = v8test.o

# Enter additional library dependencies here
ifeq ($(BUILD_WIN32),True)
LIBS	  = -lsmooth-js
else
LIBS	  = -lsmooth-js-$(VERSION)
endif

# Enter addition commands for targets all and clean here:
ALLCMD1   =
ALLCMD2   =
ALLCMD3   =
CLEANCMD1 =
CLEANCMD2 =
CLEANCMD3 =

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
