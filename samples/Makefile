########## smooth directory makefile ##########

all:
	cd beatclock && make
	cd mditest && make
	cd test && make
	cd testkey && make
ifeq ($(BUILD_WIN32),True)
	cd v8test && make
endif
#	cd widgettest && make
	cd xul && make

clean:
	cd beatclock && make clean
	cd mditest && make clean
	cd test && make clean
	cd testkey && make clean
ifeq ($(BUILD_WIN32),True)
	cd v8test && make clean
endif
#	cd widgettest && make clean
	cd xul && make clean
