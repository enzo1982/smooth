########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = mutexbackend.o semaphorebackend.o threadbackend.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd posix && make
ALLCMD2   = cd sdl && make
ALLCMD3   = cd win32 && make
CLEANCMD1 = cd posix && make clean
CLEANCMD2 = cd sdl && make clean
CLEANCMD3 = cd win32 && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
