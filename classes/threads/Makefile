########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = access.o mutex.o rwlock.o semaphore.o thread.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,backends)
CLEANCMD1 = $(call cleanin,backends)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
