########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALLIB   = libfribidi.a
MYCCOPTS    = -I$(SRCDIR) -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1

# Enter object files here:
OBJECTS	    = fribidi.o fribidi-arabic.o fribidi-bidi.o fribidi-bidi-types.o fribidi-deprecated.o fribidi-joining.o fribidi-joining-types.o fribidi-mem.o fribidi-mirroring.o fribidi-run.o fribidi-shape.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
