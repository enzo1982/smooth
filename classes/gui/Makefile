########## smooth directory $(MAKE)file ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,application)
	$(call makein,clipboard)
	$(call makein,dialogs)
	$(call makein,widgets)
	$(call makein,window)

clean:
	$(call cleanin,application)
	$(call cleanin,clipboard)
	$(call cleanin,dialogs)
	$(call cleanin,widgets)
	$(call cleanin,window)
