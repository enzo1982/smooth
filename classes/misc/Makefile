########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
ifeq ($(USE_BUNDLED_LIBICONV),True)
	MYCCOPTS += -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libiconv
else
	MYCCOPTS += -I/usr/local/include
endif

# Enter object files here:
OBJECTS	  = args.o array.o binary.o config.o datetime.o math.o memory.o number.o string.o string_case.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,encoding)
ALLCMD2   = $(call makein,hash)
CLEANCMD1 = $(call cleanin,encoding)
CLEANCMD2 = $(call cleanin,hash)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
