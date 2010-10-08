SMOOTH_PATH = ..

include $(SMOOTH_PATH)/Makefile-options

all:
	$(MAKE) -C lisa
	$(MAKE) -C libcpuid
	$(MAKE) -C fribidi2
	$(MAKE) -C nsucd
ifeq ($(BUILD_WIN32),True)
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
	$(MAKE) -C lisa clean
	$(MAKE) -C libcpuid clean
	$(MAKE) -C fribidi2 clean
	$(MAKE) -C nsucd clean
ifeq ($(BUILD_WIN32),True)
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
