SMOOTH_PATH = ../..

TARGET	    = libfribidi2.a
THISPATH    = libraries/fribidi2
OBJECTS	    = fribidi.o fribidi-bidi.o fribidi-bidi-types.o fribidi-env.o fribidi-joining.o fribidi-joining-types.o fribidi-mem.o fribidi-mirroring.o fribidi-run.o

CC = gcc
CCOPTS = -g0 -Wall -ffast-math -march=i586 -Os -I. -I$(SMOOTH_PATH)/include
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
