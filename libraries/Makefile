SMOOTH_PATH = ..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

FOLDERS =

ifeq ($(USE_BUNDLED_LIBFRIBIDI),True)
	FOLDERS += fribidi
endif

ifeq ($(USE_BUNDLED_LIBBZ2),True)
	FOLDERS += libbz2
endif

ifeq ($(USE_BUNDLED_LIBCPUID),True)
	FOLDERS += libcpuid
endif

ifeq ($(USE_BUNDLED_LIBCURL),True)
	FOLDERS += libcurl
endif

ifeq ($(USE_BUNDLED_LIBICONV),True)
	FOLDERS += libiconv
endif

ifeq ($(USE_BUNDLED_LIBJPEG),True)
	FOLDERS += libjpeg
endif

ifeq ($(USE_BUNDLED_LIBPNG),True)
	FOLDERS += libpng
endif

ifeq ($(USE_BUNDLED_LIBWEBP),True)
	FOLDERS += libwebp
endif

ifeq ($(USE_BUNDLED_LIBXML2),True)
	FOLDERS += libxml2
endif

ifeq ($(BUILD_V8),True)
	FOLDERS += v8
endif

ifeq ($(USE_BUNDLED_ZLIB),True)
	FOLDERS += zlib
endif

.PHONY: $(FOLDERS)

all: $(FOLDERS)

$(FOLDERS):
	+ $(call makein,$@)

clean: $(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean)

$(foreach FOLDER,$(FOLDERS),$(FOLDER)##clean):
	$(call cleanin,$(subst ##clean,,$@))
