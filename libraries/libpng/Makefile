########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALLIB   = libpng.a
MYCCOPTS    = -I$(SRCDIR) -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libpng -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/zlib

# Enter object files here:
OBJECTS	    = png.o pngerror.o pngget.o pngmem.o pngpread.o pngread.o pngrio.o pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o pngwutil.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
