SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

all:
	$(call makein,fribidi)
	$(call makein,libcpuid)
ifeq ($(BUILD_OSX),True)
	$(call makein,libjpeg)
ifneq ($(BUILD_XLIB),True)
	$(call makein,libpng)
	$(call makein,zlib)
endif
else ifeq ($(BUILD_QNX),True)
	$(call makein,libiconv)
	$(call makein,libjpeg)
	$(call makein,libpng)
	$(call makein,zlib)
else ifeq ($(BUILD_WIN32),True)
	$(call makein,libbz2)
	$(call makein,libiconv)
	$(call makein,libjpeg)
	$(call makein,libpng)
	$(call makein,libxml2)
	$(call makein,zlib)
endif
ifeq ($(BUILD_V8),True)
	$(call makein,v8)
endif

clean:
	$(call cleanin,fribidi)
	$(call cleanin,libcpuid)
ifeq ($(BUILD_OSX),True)
	$(call cleanin,libjpeg)
ifneq ($(BUILD_XLIB),True)
	$(call cleanin,libpng)
	$(call cleanin,zlib)
endif
else ifeq ($(BUILD_QNX),True)
	$(call cleanin,libiconv)
	$(call cleanin,libjpeg)
	$(call cleanin,libpng)
	$(call cleanin,zlib)
else ifeq ($(BUILD_WIN32),True)
	$(call cleanin,libbz2)
	$(call cleanin,libiconv)
	$(call cleanin,libjpeg)
	$(call cleanin,libpng)
	$(call cleanin,libxml2)
	$(call cleanin,zlib)
endif
ifeq ($(BUILD_V8),True)
	$(call cleanin,v8)
endif
