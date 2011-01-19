########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALLIB   = libbz2.a
MYCCOPTS    = -I. -I$(SMOOTH_PATH)/include/support/bzlib

# Enter object files here:
OBJECTS	    = blocksort.o bzlib.o compress.o crctable.o decompress.o huffman.o randtable.o

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
