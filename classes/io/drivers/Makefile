########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Enter object files here:
OBJECTS	  = driver_ansi.o driver_https.o driver_memory.o driver_posix.o driver_socket.o driver_socks4.o driver_socks5.o driver_zero.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += driver_win32.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
