########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libcpuid.a
MYCCOPTS    = -I"$(SRCDIR)" -D VERSION=\"0.6.2\" -DHAVE_STDINT_H=1

ifeq ($(BUILD_LINUX),True)
	MYCCOPTS += -D_GNU_SOURCE
endif

# Enter object files here:
OBJECTS	    = asm-bits.o cpuid_main.o libcpuid_util.o rdtsc.o recog_amd.o recog_intel.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
