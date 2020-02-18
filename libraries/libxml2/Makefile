########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libxml2.a
MYCCOPTS    = -I"$(SRCDIR)" -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libxml2 -DNOLIBTOOL -DLIBXML_STATIC -DHAVE_INTTYPES_H=1

ifeq ($(USE_BUNDLED_LIBICONV),True)
	MYCCOPTS += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libiconv
endif

# Enter object files here:
OBJECTS	    = buf.o chvalid.o dict.o encoding.o entities.o error.o globals.o hash.o list.o parser.o parserInternals.o SAX2.o threads.o tree.o uri.o valid.o xmlIO.o xmlmemory.o xmlreader.o xmlsave.o xmlstring.o xmlwriter.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
