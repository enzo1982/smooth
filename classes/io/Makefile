########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Enter object files here:
OBJECTS	  = driver.o filter.o instream.o outstream.o stream.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = cd drivers && make
ALLCMD2   = cd filters && make
CLEANCMD1 = cd drivers && make clean
CLEANCMD2 = cd filters && make clean

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
