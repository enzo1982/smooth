########## smooth directory $(MAKE)file ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,generic)
	$(call makein,image)
	$(call makein,list)
	$(call makein,menu)
	$(call makein,tree)

clean:
	$(call cleanin,generic)
	$(call cleanin,image)
	$(call cleanin,list)
	$(call cleanin,menu)
	$(call cleanin,tree)
