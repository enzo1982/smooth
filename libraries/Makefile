SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,fribidi2)
	$(call makein,libcpuid)
ifeq ($(BUILD_LINUX),True)
	$(call makein,nsucd)
else ifeq ($(BUILD_OSX),True)
	$(call makein,libjpeg)
ifneq ($(BUILD_XLIB),True)
	$(call makein,libpng)
	$(call makein,libz)
endif
	$(call makein,nsucd)
else ifeq ($(BUILD_QNX),True)
	$(call makein,libiconv)
	$(call makein,libjpeg)
	$(call makein,libpng)
	$(call makein,libz)
else ifeq ($(BUILD_WIN32),True)
	$(call makein,bzlib)
	$(call makein,libiconv)
	$(call makein,libjpeg)
	$(call makein,libpng)
	$(call makein,libxml)
	$(call makein,libz)
	$(call makein,nsucd)
endif
ifeq ($(BUILD_V8),True)
	$(call makein,v8)
endif

clean:
	$(call cleanin,fribidi2)
	$(call cleanin,libcpuid)
ifeq ($(BUILD_LINUX),True)
	$(call cleanin,nsucd)
else ifeq ($(BUILD_OSX),True)
	$(call cleanin,libjpeg)
ifneq ($(BUILD_XLIB),True)
	$(call cleanin,libpng)
	$(call cleanin,libz)
endif
	$(call cleanin,nsucd)
else ifeq ($(BUILD_QNX),True)
	$(call cleanin,libiconv)
	$(call cleanin,libjpeg)
	$(call cleanin,libpng)
	$(call cleanin,libz)
else ifeq ($(BUILD_WIN32),True)
	$(call cleanin,bzlib)
	$(call cleanin,libiconv)
	$(call cleanin,libjpeg)
	$(call cleanin,libpng)
	$(call cleanin,libxml)
	$(call cleanin,libz)
	$(call cleanin,nsucd)
endif
ifeq ($(BUILD_V8),True)
	$(call cleanin,v8)
endif
