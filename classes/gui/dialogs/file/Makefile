########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = filedlg.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += filedlg_win32.o
else
	OBJECTS += filedlg_gtk.o
endif

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
