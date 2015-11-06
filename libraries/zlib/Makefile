########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALLIB   = libz.a
MYCCOPTS    = -I$(SRCDIR) -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/zlib

# Enter object files here:
OBJECTS	    = adler32.o compress.o crc32.o deflate.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
