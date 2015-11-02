########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,beatclock)
	$(call makein,test)
	$(call makein,testkey)
ifeq ($(BUILD_WIN32),True)
	$(call makein,v8test)
endif
#	$(call makein,widgettest)
	$(call makein,xul)

clean:
	$(call cleanin,beatclock)
	$(call cleanin,test)
	$(call cleanin,testkey)
ifeq ($(BUILD_WIN32),True)
	$(call cleanin,v8test)
endif
#	$(call cleanin,widgettest)
	$(call cleanin,xul)
