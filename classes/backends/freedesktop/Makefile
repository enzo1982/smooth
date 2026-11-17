########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
OBJECTS	  = 

# Desktop settings are shared by the XLib and Wayland backends.
ifeq ($(BUILD_CAIRO),True)
	OBJECTS += desktopsettings.o

	MYCCOPTS += $(shell $(PKG_CONFIG) --cflags gtk+-3.0)
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
