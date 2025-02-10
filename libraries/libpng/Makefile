########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libpng.a
MYCCOPTS    = -I"$(SRCDIR)" -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/libpng

ifeq ($(USE_BUNDLED_ZLIB),True)
MYCCOPTS   += -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support/zlib
endif

ifeq ($(BUILD_ARM),True)
    MYCCOPTS += -DPNG_ARM_NEON_OPT=0
else ifeq ($(BUILD_ARM64),True)
    MYCCOPTS += -DPNG_ARM_NEON_OPT=0
else ifeq ($(BUILD_PPC64),True)
    MYCCOPTS += -DPNG_POWERPC_VSX_OPT=0
endif

# Enter object files here:
OBJECTS	    = png.o pngerror.o pngget.o pngmem.o pngpread.o pngread.o pngrio.o pngrtran.o pngrutil.o pngset.o pngtrans.o pngwio.o pngwrite.o pngwtran.o pngwutil.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
