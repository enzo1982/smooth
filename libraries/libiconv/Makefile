########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options
 
# Change these variables to fit your project:
ifeq ($(BUILD_WIN32),True)
	GLOBALDLL   = iconv
	IMPORTLIB   = libiconv.a
else
	GLOBALLIB   = libiconv.a
endif

MYCCOPTS    = -I$(SRCDIR) -I$(SRCDIR)/$(SMOOTH_PATH)/include/support/libiconv

ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -D_ICONVDLL_
else
	MYCCOPTS += -DLIBDIR=\"/usr/lib\"
endif

# Enter object files here:
OBJECTS	    = iconv.o localcharset.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
