########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

# Enter object files here:
OBJECTS	  = clipboardbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd cocoa && $(MAKE)
ALLCMD2   = cd haiku && $(MAKE)
ALLCMD3   = cd photon && $(MAKE)
ALLCMD4   = cd win32 && $(MAKE)
ALLCMD5   = cd xlib && $(MAKE)
CLEANCMD1 = cd cocoa && $(MAKE) clean
CLEANCMD2 = cd haiku && $(MAKE) clean
CLEANCMD3 = cd photon && $(MAKE) clean
CLEANCMD4 = cd win32 && $(MAKE) clean
CLEANCMD5 = cd xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
