########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
ifeq ($(BUILD_WIN32),True)
	GLOBALDLL   = iconv
	IMPORTLIB   = libiconv.a
else
	GLOBALLIB   = libiconv.a
endif

MYCCOPTS    = -DLIBICONV_PLUG -I.

ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -D_ICONVDLL_
else
	MYCCOPTS += -DLIBDIR=\"/usr/lib\"
endif

# Enter object files here:
OBJECTS	    = iconv.o localcharset.o

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands
