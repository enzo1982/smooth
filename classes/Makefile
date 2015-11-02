########## smooth directory $(MAKE)file ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,backends)
	$(call makein,basic)
	$(call makein,errors)
	$(call makein,files)
	$(call makein,graphics)
	$(call makein,gui)
	$(call makein,i18n)
	$(call makein,input)
	$(call makein,io)
	$(call makein,misc)
	$(call makein,net)
	$(call makein,system)
	$(call makein,threads)
	$(call makein,xml)

clean:
	$(call cleanin,backends)
	$(call cleanin,basic)
	$(call cleanin,errors)
	$(call cleanin,files)
	$(call cleanin,graphics)
	$(call cleanin,gui)
	$(call cleanin,i18n)
	$(call cleanin,input)
	$(call cleanin,io)
	$(call cleanin,misc)
	$(call cleanin,net)
	$(call cleanin,system)
	$(call cleanin,threads)
	$(call cleanin,xml)
