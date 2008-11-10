SMOOTH_PATH = ../..

TARGET	    = libiconv.a
THISPATH    = libraries/libiconv
OBJECTS	    = iconv.o localcharset.o

CC = gcc
CCOPTS = -g0 -Wall -ffast-math -march=i586 -Os -DLIBICONV_PLUG -D_ICONVDLL_ -I. -I$(SMOOTH_PATH)/include
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
