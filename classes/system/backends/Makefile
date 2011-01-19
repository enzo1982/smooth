########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = eventbackend.o timerbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd posix && $(MAKE)
ALLCMD2   = cd threads && $(MAKE)
ALLCMD3   = cd win32 && $(MAKE)
ALLCMD4   = cd xlib && $(MAKE)
CLEANCMD1 = cd posix && $(MAKE) clean
CLEANCMD2 = cd threads && $(MAKE) clean
CLEANCMD3 = cd win32 && $(MAKE) clean
CLEANCMD4 = cd xlib && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
