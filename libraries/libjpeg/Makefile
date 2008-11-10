SMOOTH_PATH = ../..

TARGET	    = libjpeg.a
THISPATH    = libraries/libjpeg
OBJECTS	    = jcapimin.o jcapistd.o jccoefct.o jccolor.o jcdctmgr.o jchuff.o jcinit.o jcmainct.o jcmarker.o jcmaster.o jcomapi.o jcparam.o jcphuff.o jcprepct.o jcsample.o jctrans.o jdapimin.o jdapistd.o jdatadst.o jdatasrc.o jdcoefct.o jdcolor.o jddctmgr.o jdhuff.o jdinput.o jdmainct.o jdmarker.o jdmaster.o jdmerge.o jdphuff.o jdpostct.o jdsample.o jdtrans.o jerror.o jfdctflt.o jfdctfst.o jfdctint.o jidctflt.o jidctfst.o jidctint.o jidctred.o jmemmgr.o jmemnobs.o jquant1.o jquant2.o jutils.o

CC = gcc
CCOPTS = -g0 -Wall -ffast-math -march=i586 -Os -I. -I$(SMOOTH_PATH)/include/libjpeg
REMOVER = rm
REMOVER_OPTS = -f
ECHO = echo
AR = ar

.SILENT:

all : $(SMOOTH_PATH)/$(LIBDIR)/$(TARGET)

$(SMOOTH_PATH)/$(LIBDIR)/$(TARGET) : $(OBJECTS)
	$(ECHO) -n Linking $(LIBDIR)/$(TARGET)...
	$(AR) r $@ $(OBJECTS) &> $(SMOOTH_PATH)/NULL
	$(ECHO) done.

clean :
	$(ECHO) -n Cleaning $(THISPATH)...
	$(REMOVER) $(REMOVER_OPTS) $(SMOOTH_PATH)/$(LIBDIR)/$(TARGET) $(OBJECTS)
	$(ECHO) done.

.c.o:
	$(ECHO) -n Compiling $(THISPATH)/$<...
	$(CC) $(CCOPTS) -c $< -o $@
	$(ECHO) done.
