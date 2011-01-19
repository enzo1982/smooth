########## smooth directory makefile ##########

all:
	cd beatclock && $(MAKE)
	cd mditest && $(MAKE)
	cd test && $(MAKE)
	cd testkey && $(MAKE)
ifeq ($(BUILD_WIN32),True)
	cd v8test && $(MAKE)
endif
#	cd widgettest && $(MAKE)
	cd xul && $(MAKE)

clean:
	cd beatclock && $(MAKE) clean
	cd mditest && $(MAKE) clean
	cd test && $(MAKE) clean
	cd testkey && $(MAKE) clean
ifeq ($(BUILD_WIN32),True)
	cd v8test && $(MAKE) clean
endif
#	cd widgettest && $(MAKE) clean
	cd xul && $(MAKE) clean
