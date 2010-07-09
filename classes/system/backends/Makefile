########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = eventbackend.o timerbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd sdl && make
ALLCMD2   = cd threads && make
ALLCMD3   = cd win32 && make
ALLCMD4   = cd xlib && make
CLEANCMD1 = cd sdl && make clean
CLEANCMD2 = cd threads && make clean
CLEANCMD3 = cd win32 && make clean
CLEANCMD4 = cd xlib && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
