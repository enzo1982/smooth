########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_HAIKU),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -I/usr/X11/include
else ifeq ($(BUILD_QNX),True)
	MYCCOPTS += -I/usr/X11R6/include
else
	MYCCOPTS += $(shell pkg-config --cflags x11)
endif
endif
endif

# Enter object files here:
OBJECTS	  = console.o cpu.o dynamicloader.o event.o screen.o system.o timer.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,backends)
CLEANCMD1 = $(call cleanin,backends)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
