########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifneq ($(BUILD_WIN32),True)
ifneq ($(BUILD_OSX),True)
ifneq ($(BUILD_HAIKU),True)
	MYCCOPTS += $(shell pkg-config --cflags gtk+-3.0)
endif
endif
endif

# Enter object files here:
OBJECTS	  = filedlg.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += filedlg_win32.o
else ifeq ($(BUILD_OSX),True)
	OBJECTS += filedlg_cocoa.o
else ifeq ($(BUILD_HAIKU),True)
	OBJECTS += filedlg_haiku.o
else
	OBJECTS += filedlg_gtk.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
