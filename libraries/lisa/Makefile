########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = liblisa.a

# Enter object files here:
OBJECTS	    = 

ifeq ($(BUILD_WIN32),True)
	OBJECTS += lisa_win32.o
endif

ifeq ($(BUILD_LINUX),True)
	OBJECTS += lisa_linux.o
endif

ifeq ($(BUILD_QNX),True)
	OBJECTS += lisa_qnx.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
