########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = mutexbackend.o semaphorebackend.o threadbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd posix && $(MAKE)
ALLCMD2   = cd win32 && $(MAKE)
CLEANCMD1 = cd posix && $(MAKE) clean
CLEANCMD2 = cd win32 && $(MAKE) clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
