SMOOTH_PATH = ..

include $(SMOOTH_PATH)/Makefile-options

all:
	$(MAKE) -C fribidi2
	$(MAKE) -C libcpuid
ifeq ($(BUILD_LINUX),True)
	$(MAKE) -C nsucd
else ifeq ($(BUILD_OSX),True)
	$(MAKE) -C libjpeg
ifneq ($(BUILD_XLIB),True)
	$(MAKE) -C libpng
	$(MAKE) -C libz
endif
	$(MAKE) -C nsucd
else ifeq ($(BUILD_QNX),True)
	$(MAKE) -C libiconv
	$(MAKE) -C libjpeg
	$(MAKE) -C libpng
	$(MAKE) -C libz
else ifeq ($(BUILD_WIN32),True)
	$(MAKE) -C bzlib
	$(MAKE) -C libiconv
	$(MAKE) -C libjpeg
	$(MAKE) -C libpng
	$(MAKE) -C libxml
	$(MAKE) -C libz
	$(MAKE) -C lzma
	$(MAKE) -C mzip
	$(MAKE) -C nsucd
	$(MAKE) -C pcre
endif
ifeq ($(BUILD_V8),True)
	$(MAKE) -C v8
endif

clean:
	$(MAKE) -C fribidi2 clean
	$(MAKE) -C libcpuid clean
ifeq ($(BUILD_LINUX),True)
	$(MAKE) -C nsucd clean
else ifeq ($(BUILD_OSX),True)
	$(MAKE) -C libjpeg clean
ifneq ($(BUILD_XLIB),True)
	$(MAKE) -C libpng clean
	$(MAKE) -C libz clean
endif
	$(MAKE) -C nsucd clean
else ifeq ($(BUILD_QNX),True)
	$(MAKE) -C libiconv clean
	$(MAKE) -C libjpeg clean
	$(MAKE) -C libpng clean
	$(MAKE) -C libz clean
else ifeq ($(BUILD_WIN32),True)
	$(MAKE) -C bzlib clean
	$(MAKE) -C libiconv clean
	$(MAKE) -C libjpeg clean
	$(MAKE) -C libpng clean
	$(MAKE) -C libxml clean
	$(MAKE) -C libz clean
	$(MAKE) -C lzma clean
	$(MAKE) -C mzip clean
	$(MAKE) -C nsucd clean
	$(MAKE) -C pcre clean
endif
ifeq ($(BUILD_V8),True)
	$(MAKE) -C v8 clean
endif
