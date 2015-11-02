########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,designer)
	$(call makein,pciffmanager)
	$(call makein,translator)

clean:
	$(call cleanin,designer)
	$(call cleanin,pciffmanager)
	$(call cleanin,translator)
