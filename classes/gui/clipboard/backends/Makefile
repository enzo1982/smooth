########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

# Enter object files here:
OBJECTS	  = clipboardbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd cocoa && $(MAKE) && cd ../haiku && $(MAKE)
ALLCMD2   = cd photon && $(MAKE) && cd ../win32 && $(MAKE)
ALLCMD3   = cd xlib && $(MAKE)
CLEANCMD1 = cd cocoa && $(MAKE) clean && cd ../haiku && $(MAKE) clean
CLEANCMD2 = cd photon && $(MAKE) clean && cd ../win32 && $(MAKE) clean
CLEANCMD3 = cd xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
