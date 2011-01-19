SMOOTH_PATH = ..

include $(SMOOTH_PATH)/Makefile-options

all:
	$(MAKE) -C libcpuid
	$(MAKE) -C fribidi2
ifeq ($(BUILD_LINUX),True)
	$(MAKE) -C nsucd
endif
ifeq ($(BUILD_OSX),True)
	$(MAKE) -C nsucd
endif
ifeq ($(BUILD_WIN32),True)
	$(MAKE) -C nsucd
	$(MAKE) -C bzlib
	$(MAKE) -C libiconv
	$(MAKE) -C libjpeg
	$(MAKE) -C libpng
	$(MAKE) -C libxml
	$(MAKE) -C libz
	$(MAKE) -C lzma
	$(MAKE) -C mzip
	$(MAKE) -C pcre
ifeq ($(BUILD_V8),True)
	$(MAKE) -C v8
endif
endif

clean:
	$(MAKE) -C libcpuid clean
	$(MAKE) -C fribidi2 clean
ifeq ($(BUILD_LINUX),True)
	$(MAKE) -C nsucd clean
endif
ifeq ($(BUILD_OSX),True)
	$(MAKE) -C nsucd clean
endif
ifeq ($(BUILD_WIN32),True)
	$(MAKE) -C nsucd clean
	$(MAKE) -C bzlib clean
	$(MAKE) -C libiconv clean
	$(MAKE) -C libjpeg clean
	$(MAKE) -C libpng clean
	$(MAKE) -C libxml clean
	$(MAKE) -C libz clean
	$(MAKE) -C lzma clean
	$(MAKE) -C mzip clean
	$(MAKE) -C pcre clean
ifeq ($(BUILD_V8),True)
	$(MAKE) -C v8 clean
endif
endif
