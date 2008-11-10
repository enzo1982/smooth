SMOOTH_PATH = ../..

TARGET	    = libxml.a
THISPATH    = libraries/libxml
OBJECTS	    = chvalid.o dict.o encoding.o entities.o error.o globals.o hash.o list.o parser.o parserInternals.o SAX2.o threads.o tree.o uri.o valid.o xmlIO.o xmlmemory.o xmlreader.o xmlsave.o xmlstring.o xmlwriter.o

CC = gcc
CCOPTS = -g0 -Wall -ffast-math -march=i586 -Os -DLIBICONV_PLUG -DLIBXML_STATIC -I. -I$(SMOOTH_PATH)/include
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
