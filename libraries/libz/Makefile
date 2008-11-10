SMOOTH_PATH = ../..

TARGET	    = libz.a
THISPATH    = libraries/libz
OBJECTS	    = adler32.o compress.o crc32.o deflate.o gzio.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o

CC = gcc
CCOPTS = -g0 -Wall -ffast-math -march=i586 -Os -I. -I$(SMOOTH_PATH)/include/libz
REMOVER = rm
REMOVER_OPTS = -f
ECHO = echo
AR = ar

.SILENT:

all : $(SMOOTH_PATH)/$(LIBDIR)/$(TARGET)

$(SMOOTH_PATH)/$(LIBDIR)/$(TARGET): $(OBJECTS)
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
