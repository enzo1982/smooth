# Makefile for smooth v0.8

INCLUDEDIR = ./include
SMOOTHINCLUDEDIR = $(INCLUDEDIR)/smooth
OBJECTDIR = ./objects
BINDIR = ./bin
LIBDIR = ./lib
OTHERLIBSDIR = ./libraries
IOLIBDIR = $(OTHERLIBSDIR)/iolib
PICTUREDIR = $(OTHERLIBSDIR)/picture
LIBPCIDIR = $(OTHERLIBSDIR)/libpci
IOLIBFILTERSDIR = $(OTHERLIBSDIR)/iolib-filters
LIBBZ2DIR = $(OTHERLIBSDIR)/bzlib
LIBXMLDIR = $(OTHERLIBSDIR)/libxml
LIBICONVDIR = $(OTHERLIBSDIR)/libiconv
LISADIR = $(OTHERLIBSDIR)/lisa
MISCDIR = ./misc
DIALOGSDIR = ./dialogs
CLASSESDIR = ./classes
WINDOWCLASSESDIR = $(CLASSESDIR)/window
TREECLASSESDIR = $(CLASSESDIR)/tree
MENUCLASSESDIR = $(CLASSESDIR)/menu
LISTCLASSESDIR = $(CLASSESDIR)/list
LAYERCLASSESDIR = $(CLASSESDIR)/layer
BASICCLASSESDIR = $(CLASSESDIR)/basic
MDICLASSESDIR = $(CLASSESDIR)/mdi
MISCCLASSESDIR = $(CLASSESDIR)/misc
INTERNALCLASSESDIR = $(CLASSESDIR)/internal
THREADSCLASSESDIR = $(CLASSESDIR)/threads
XMLCLASSESDIR = $(CLASSESDIR)/xml
PRIMITIVESCLASSESDIR = $(CLASSESDIR)/primitives
I18NDIR = ./i18n
SAMPLESDIR = ./samples
SAMPLE1DIR = $(SAMPLESDIR)/beatclock
SAMPLE2DIR = $(SAMPLESDIR)/test
SAMPLE3DIR = $(SAMPLESDIR)/testkey
SAMPLE4DIR = $(SAMPLESDIR)/plugin
SAMPLE5DIR = $(SAMPLESDIR)/mditest
TUTORIALDIR = ./tutorial
TUTORIAL01DIR = $(TUTORIALDIR)/t01
TUTORIAL02DIR = $(TUTORIALDIR)/t02
DESIGNERDIR = ./tools/designer
TESTDIR = ./test
OBJTESTDIR = $(TESTDIR)/objtest
RESOURCEDIR = ./resources
BINRESDIR = $(RESOURCEDIR)/binary
PACKAGEDIR = ./package
SESSIONSDIR = $(BINDIR)/sessions

OBJECTS = $(OBJECTDIR)/application.o $(OBJECTDIR)/background.o $(OBJECTDIR)/container.o $(OBJECTDIR)/containertype.o $(OBJECTDIR)/object.o $(OBJECTDIR)/objectmanager.o $(OBJECTDIR)/objectproperties.o $(OBJECTDIR)/objecttype.o $(OBJECTDIR)/toolwindow.o $(OBJECTDIR)/activearea.o $(OBJECTDIR)/arrows.o $(OBJECTDIR)/bitmap.o $(OBJECTDIR)/button.o $(OBJECTDIR)/checkbox.o $(OBJECTDIR)/editbox.o $(OBJECTDIR)/groupbox.o $(OBJECTDIR)/hyperlink.o $(OBJECTDIR)/layer.o $(OBJECTDIR)/optionbox.o $(OBJECTDIR)/progressbar.o $(OBJECTDIR)/scrollbar.o $(OBJECTDIR)/slider.o $(OBJECTDIR)/tabregister.o $(OBJECTDIR)/text.o $(OBJECTDIR)/combobox.o $(OBJECTDIR)/list.o $(OBJECTDIR)/listbox.o $(OBJECTDIR)/listentry.o $(OBJECTDIR)/mdiclient.o $(OBJECTDIR)/mdiwindow.o $(OBJECTDIR)/menu.o $(OBJECTDIR)/menubar.o $(OBJECTDIR)/menuentry.o $(OBJECTDIR)/popupmenu.o $(OBJECTDIR)/popupview.o $(OBJECTDIR)/date.o $(OBJECTDIR)/string.o $(OBJECTDIR)/surface.o $(OBJECTDIR)/surfacegdi.o $(OBJECTDIR)/time.o $(OBJECTDIR)/timer.o $(OBJECTDIR)/line.o $(OBJECTDIR)/point.o $(OBJECTDIR)/primitive.o $(OBJECTDIR)/rect.o $(OBJECTDIR)/size.o $(OBJECTDIR)/mutex.o $(OBJECTDIR)/semaphore.o $(OBJECTDIR)/thread.o $(OBJECTDIR)/threadmanager.o $(OBJECTDIR)/tree.o $(OBJECTDIR)/treeentry.o $(OBJECTDIR)/treeview.o $(OBJECTDIR)/client.o $(OBJECTDIR)/divisionbar.o $(OBJECTDIR)/drag.o $(OBJECTDIR)/statusbar.o $(OBJECTDIR)/titlebar.o $(OBJECTDIR)/tooltip.o $(OBJECTDIR)/window.o $(OBJECTDIR)/xml_attribute.o $(OBJECTDIR)/xml_document.o $(OBJECTDIR)/xml_node.o $(OBJECTDIR)/colordlg.o $(OBJECTDIR)/dialog.o $(OBJECTDIR)/dirdlg.o $(OBJECTDIR)/filedlg.o $(OBJECTDIR)/afrikaans.o $(OBJECTDIR)/basque.o $(OBJECTDIR)/breton.o $(OBJECTDIR)/bulgarian.o $(OBJECTDIR)/catalan.o $(OBJECTDIR)/chinese.o $(OBJECTDIR)/chinese-simplified.o $(OBJECTDIR)/croatian.o $(OBJECTDIR)/czech.o $(OBJECTDIR)/danish.o $(OBJECTDIR)/dutch.o $(OBJECTDIR)/english.o $(OBJECTDIR)/esperanto.o $(OBJECTDIR)/estonian.o $(OBJECTDIR)/finnish.o $(OBJECTDIR)/french.o $(OBJECTDIR)/galician.o $(OBJECTDIR)/german.o $(OBJECTDIR)/greek.o $(OBJECTDIR)/hebrew.o $(OBJECTDIR)/hungarian.o $(OBJECTDIR)/icelandic.o $(OBJECTDIR)/irish.o $(OBJECTDIR)/italian.o $(OBJECTDIR)/japanese.o $(OBJECTDIR)/korean.o $(OBJECTDIR)/latin.o $(OBJECTDIR)/latvian.o $(OBJECTDIR)/lithuanian.o $(OBJECTDIR)/macedonian.o $(OBJECTDIR)/maori.o $(OBJECTDIR)/norwegian.o $(OBJECTDIR)/occitan.o $(OBJECTDIR)/polish.o $(OBJECTDIR)/portuguese.o $(OBJECTDIR)/portuguese-brazil.o $(OBJECTDIR)/romanian.o $(OBJECTDIR)/russian.o $(OBJECTDIR)/serbian.o $(OBJECTDIR)/slovak.o $(OBJECTDIR)/slovenian.o $(OBJECTDIR)/spanish.o $(OBJECTDIR)/swedish.o $(OBJECTDIR)/tamil.o $(OBJECTDIR)/thai.o $(OBJECTDIR)/turkish.o $(OBJECTDIR)/ukrainian.o $(OBJECTDIR)/walloon.o $(OBJECTDIR)/welsh.o $(OBJECTDIR)/i18n.o $(OBJECTDIR)/binary.o $(OBJECTDIR)/color.o $(OBJECTDIR)/definitions.o $(OBJECTDIR)/dllmain.o $(OBJECTDIR)/loop.o $(OBJECTDIR)/mathtools.o $(OBJECTDIR)/messagebox.o $(OBJECTDIR)/metrics.o $(OBJECTDIR)/splashscreen.o $(OBJECTDIR)/stk.o $(OBJECTDIR)/toolkit.o
RESOURCES = $(OBJECTDIR)/resources.o

LIBS = -mwindows -lpci -liolib-filters -lbz2-mini -lpicture -llisa -lxml -lwsock32 -liconv

SAMPLESLIBS = -lsmooth

DLLNAME = $(BINDIR)/smooth.dll
LIBNAME = $(LIBDIR)/libsmooth.a
EXPNAME = $(LIBDIR)/smooth.exp
DEFNAME = $(LIBDIR)/smooth.def

LIBIOLIB = $(LIBDIR)/libiolib.a
LIBPICTURE = $(LIBDIR)/libpicture.a
LIBPCI = $(LIBDIR)/libpci.a
LIBIOLIBFILTERS = $(LIBDIR)/libiolib-filters.a
LIBBZ2 = $(LIBDIR)/libbz2-mini.a
LIBXML = $(LIBDIR)/libxml.a
LIBICONV = $(LIBDIR)/libiconv.a
LIBLISA = $(LIBDIR)/liblisa.a

OTHERLIBS = $(LIBIOLIB) $(LIBPICTURE) $(LIBPCI) $(LIBIOLIBFILTERS) $(LIBBZ2) $(LIBXML) $(LIBICONV) $(LIBLISA)

IOLIBOBJECTS = $(OBJECTDIR)/iolib_instream.o $(OBJECTDIR)/iolib_iolib.o $(OBJECTDIR)/iolib_iolibdriver.o $(OBJECTDIR)/iolib_iolibfilter.o $(OBJECTDIR)/iolib_iolibstream.o $(OBJECTDIR)/iolib_outstream.o
PICTUREOBJECTS = $(OBJECTDIR)/picture_picture.o
LIBPCIOBJECTS = $(OBJECTDIR)/libpci_codecs.o $(OBJECTDIR)/libpci_pciio.o
IOLIBFILTERSOBJECTS = $(OBJECTDIR)/iolib_filter-bz2.o $(OBJECTDIR)/iolib_filter-xor.o $(OBJECTDIR)/iolib_iolib-filters.o
LIBBZ2OBJECTS = $(OBJECTDIR)/libbz2_blocksort.o $(OBJECTDIR)/libbz2_bzlib.o $(OBJECTDIR)/libbz2_compress.o $(OBJECTDIR)/libbz2_crctable.o $(OBJECTDIR)/libbz2_decompress.o $(OBJECTDIR)/libbz2_huffman.o $(OBJECTDIR)/libbz2_randtable.o
LIBXMLOBJECTS = $(OBJECTDIR)/libxml_catalog.o $(OBJECTDIR)/libxml_encoding.o $(OBJECTDIR)/libxml_entities.o $(OBJECTDIR)/libxml_error.o $(OBJECTDIR)/libxml_globals.o $(OBJECTDIR)/libxml_hash.o $(OBJECTDIR)/libxml_list.o $(OBJECTDIR)/libxml_parser.o $(OBJECTDIR)/libxml_parserInternals.o $(OBJECTDIR)/libxml_SAX.o $(OBJECTDIR)/libxml_threads.o $(OBJECTDIR)/libxml_tree.o $(OBJECTDIR)/libxml_uri.o $(OBJECTDIR)/libxml_valid.o $(OBJECTDIR)/libxml_xlink.o $(OBJECTDIR)/libxml_xmlIO.o $(OBJECTDIR)/libxml_xmlmemory.o $(OBJECTDIR)/libxml_xpath.o $(OBJECTDIR)/libxml_xpointer.o
LIBICONVOBJECTS = $(OBJECTDIR)/libiconv_iconv.o $(OBJECTDIR)/libiconv_localcharset.o
LISAOBJECTS = $(OBJECTDIR)/lisa_lisa.o

DESIGNEROBJECTS = $(OBJECTDIR)/designer.o $(OBJECTDIR)/edit_component.o $(OBJECTDIR)/tools.o
OBJTESTOBJECTS = $(OBJTESTDIR)/objtest.o
SAMPLE1OBJECTS = $(SAMPLE1DIR)/beat.o
SAMPLE2OBJECTS = $(SAMPLE2DIR)/test.o
SAMPLE3OBJECTS = $(SAMPLE3DIR)/testkey.o
SAMPLE4OBJECTS = $(SAMPLE4DIR)/plugintest.o
SAMPLE5OBJECTS = $(SAMPLE5DIR)/mditest.o
SAMPLE4DLLOBJECTS = $(SAMPLE4DIR)/plugin.o

TUTORIAL01OBJECTS = $(TUTORIAL01DIR)/t01.o
TUTORIAL02OBJECTS = $(TUTORIAL02DIR)/t02.o

DESIGNEREXE = $(BINDIR)/designer.exe
OBJTESTEXE = $(OBJTESTDIR)/objtest.exe
SAMPLE1EXE = $(SAMPLE1DIR)/beat.exe
SAMPLE2EXE = $(SAMPLE2DIR)/test.exe
SAMPLE3EXE = $(SAMPLE3DIR)/testkey.exe
SAMPLE4EXE = $(SAMPLE4DIR)/plugintest.exe
SAMPLE5EXE = $(SAMPLE5DIR)/mditest.exe
SAMPLE4DLL = $(SAMPLE4DIR)/plugin.dll
SAMPLE4LIB = $(SAMPLE4DIR)/libplugin.a
SAMPLE4EXP = $(SAMPLE4DIR)/plugin.exp
SAMPLE4DEF = $(SAMPLE4DIR)/plugin.def

TUTORIAL01EXE = $(TUTORIAL01DIR)/t01.exe
TUTORIAL02EXE = $(TUTORIAL02DIR)/t02.exe

SETUPEXE = $(PACKAGEDIR)/smooth.exe

COMPILER = gcc
LINKER = dllwrap
EXELINKER = gcc
RESCOMP = windres
STRIP = strip
REMOVER = rm
COPIER = cp
ECHO = echo
TAR = tar
AR = ar
PACKER = upx
COMPILER_OPTS = -I$(INCLUDEDIR) -march=i586 -O6 -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -D__SMOOTH_DLL__ -DIOLIB_DLL -c
LINKER_OPTS = -L$(LIBDIR) -mwindows --output-lib $(LIBNAME) --output-exp $(EXPNAME) --output-def $(DEFNAME) -o $(DLLNAME)
PLUGIN_LINKER_OPTS = -L$(LIBDIR) -mwindows --output-lib $(SAMPLE4LIB) --output-exp $(SAMPLE4EXP) --output-def $(SAMPLE4DEF) -o $(SAMPLE4DLL)
EXECOMPILER_OPTS = -I$(INCLUDEDIR) -march=i586 -O6 -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -c
EXELINKER_OPTS = -L$(LIBDIR) $(SAMPLESLIBS) -mwindows
RESCOMP_OPTS = -D__SMOOTH_DLL__ -O coff
STRIP_OPTS = --strip-all
LIBSTRIP_OPTS = --strip-debug
REMOVER_OPTS = -f
COPIER_OPTS = -f
PACKER_OPTS = -9 -q

.PHONY: all install clean distribute tutorial tutorial_distribute
.SILENT:

all: $(OBJECTS) $(RESOURCES) $(OTHERLIBS) $(DLLNAME) $(DESIGNEREXE) $(OBJTESTEXE) $(SAMPLE1EXE) $(SAMPLE2EXE) $(SAMPLE3EXE) $(SAMPLE4DLL) $(SAMPLE4EXE) $(SAMPLE5EXE) tutorial tutorial_distribute distribute

tutorial: $(TUTORIAL01EXE) $(TUTORIAL02EXE) tutorial_distribute

install:

clean:
	$(ECHO) -n Cleaning directories...
	$(REMOVER) $(REMOVER_OPTS) $(OTHERLIBS) $(IOLIBOBJECTS) $(IOLIBFILTERSOBJECTS) $(LIBBZ2OBJECTS) $(LIBPCIOBJECTS) $(PICTUREOBJECTS) $(LIBXMLOBJECTS) $(LIBICONVOBJECTS) $(LISAOBJECTS)
	$(REMOVER) $(REMOVER_OPTS) $(OBJECTS) $(RESOURCES) $(DLLNAME) $(LIBNAME) $(EXPNAME) $(DEFNAME) $(DESIGNEREXE) $(OBJTESTEXE) $(SAMPLE1EXE) $(SAMPLE2EXE) $(SAMPLE3EXE) $(SAMPLE4EXE) $(SAMPLE5EXE) $(DESIGNEROBJECTS) $(OBJTESTOBJECTS) $(SAMPLE1OBJECTS) $(SAMPLE2OBJECTS) $(SAMPLE3OBJECTS) $(SAMPLE4OBJECTS) $(SAMPLE5OBJECTS) $(SAMPLE4DLLOBJECTS) $(SAMPLE4DLL) $(SAMPLE4LIB) $(SAMPLE4DEF) $(SAMPLE4EXP) $(TUTORIAL01OBJECTS) $(TUTORIAL02OBJECTS) $(TUTORIAL01EXE) $(TUTORIAL02EXE)
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/kit.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/source.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/misc.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/samples.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/smooth.exe
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(OBJTESTDIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(SAMPLE1DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(SAMPLE2DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(SAMPLE3DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(SAMPLE4DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(SAMPLE5DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(TUTORIAL01DIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(TUTORIAL02DIR)/smooth.dll
	$(ECHO) done.

distribute: $(DLLNAME) $(DESIGNEREXE) $(OBJTESTEXE) $(SAMPLE1EXE) $(SAMPLE2EXE) $(SAMPLE3EXE)
	$(ECHO) -n Distributing files to directories...
	$(REMOVER) $(REMOVER_OPTS) NULL
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(OBJTESTDIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(SAMPLE1DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(SAMPLE2DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(SAMPLE3DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(SAMPLE4DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(SAMPLE5DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(PACKAGEDIR)/smooth.dll
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/kit.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/source.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/misc.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/samples.tar.gz
	$(REMOVER) $(REMOVER_OPTS) $(PACKAGEDIR)/smooth.exe
	$(TAR) c --gzip --file=$(PACKAGEDIR)/kit.tar.gz include bin/smooth.dll lib/smooth.exp lib/smooth.def lib/libsmooth.a bin/designer.exe test/objtest/objtest.exe
	$(TAR) c --exclude=*.o --gzip --file=$(PACKAGEDIR)/source.tar.gz objects classes dialogs i18n include libraries misc resources tools test/objtest/objtest.cpp test/objtest/objtest.h Makefile package/files.dat package/psa.exe package/psa.sh package/setup.exe package/setup.psi package/smooth.exe.orig
	$(TAR) c --gzip --file=$(PACKAGEDIR)/misc.tar.gz doc BugList ChangeLog ChangeLog.2001 ChangeLog.2000 ChangeLog.1999 Compatibility ToDo tutorial/index.html tutorial/t01/t01.cpp tutorial/t01/t01.h tutorial/t01/t01.exe tutorial/t02/t02.cpp tutorial/t02/t02.h tutorial/t02/t02.exe
	$(TAR) c --gzip --file=$(PACKAGEDIR)/samples.tar.gz samples/beatclock/beat.cpp samples/beatclock/beat.h samples/beatclock/beat.exe samples/beatclock/beat.pci samples/test/test.cpp samples/test/test.h samples/test/test.exe samples/test/icons.pci samples/mditest/mditest.cpp samples/mditest/mditest.h samples/mditest/mditest.exe samples/testkey/testkey.cpp samples/testkey/testkey.h samples/testkey/testkey.exe samples/plugin/plugintest.exe samples/plugin/plugintest.cpp samples/plugin/plugintest.h samples/plugin/plugin.dll samples/plugin/plugin.cpp samples/plugin/plugin.h samples/plugin/plugin.def samples/plugin/plugin.exp samples/plugin/libplugin.a
	$(COPIER) $(COPIER_OPTS) $(PACKAGEDIR)/smooth.exe.orig $(PACKAGEDIR)/smooth.exe
	bash $(PACKAGEDIR)/psa.sh
	$(ECHO) done.

tutorial_distribute:
	$(ECHO) -n Distributing files to directories...
	$(REMOVER) $(REMOVER_OPTS) NULL
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(TUTORIAL01DIR)/smooth.dll
	$(COPIER) $(COPIER_OPTS) $(DLLNAME) $(TUTORIAL02DIR)/smooth.dll
	$(ECHO) done.

$(DLLNAME): $(OBJECTS) $(RESOURCES) $(OTHERLIBS)
	$(ECHO) Linking $(DLLNAME)...
	$(LINKER) $(OBJECTS) $(LIBIOLIB) $(RESOURCES) $(LINKER_OPTS) $(LIBS) > NULL
	$(STRIP) $(STRIP_OPTS) $(DLLNAME)
	$(STRIP) $(LIBSTRIP_OPTS) $(LIBNAME)
	$(PACKER) $(PACKER_OPTS) $(DLLNAME) > NULL
	$(ECHO) done.

$(LIBIOLIB): $(IOLIBOBJECTS)
	$(ECHO) -n Linking $(LIBIOLIB)...
	$(AR) r $(LIBIOLIB) $(IOLIBOBJECTS)
	$(ECHO) done.

$(LIBPICTURE): $(PICTUREOBJECTS)
	$(ECHO) -n Linking $(LIBPICTURE)...
	$(AR) r $(LIBPICTURE) $(PICTUREOBJECTS)
	$(ECHO) done.

$(LIBPCI): $(LIBPCIOBJECTS)
	$(ECHO) -n Linking $(LIBPCI)...
	$(AR) r $(LIBPCI) $(LIBPCIOBJECTS)
	$(ECHO) done.

$(LIBIOLIBFILTERS): $(IOLIBFILTERSOBJECTS)
	$(ECHO) -n Linking $(LIBIOLIBFILTERS)...
	$(AR) r $(LIBIOLIBFILTERS) $(IOLIBFILTERSOBJECTS)
	$(ECHO) done.

$(LIBBZ2): $(LIBBZ2OBJECTS)
	$(ECHO) -n Linking $(LIBBZ2)...
	$(AR) r $(LIBBZ2) $(LIBBZ2OBJECTS)
	$(ECHO) done.

$(LIBXML): $(LIBXMLOBJECTS)
	$(ECHO) -n Linking $(LIBXML)...
	$(AR) r $(LIBXML) $(LIBXMLOBJECTS)
	$(ECHO) done.

$(LIBICONV): $(LIBICONVOBJECTS)
	$(ECHO) -n Linking $(LIBICONV)...
	$(AR) r $(LIBICONV) $(LIBICONVOBJECTS)
	$(ECHO) done.

$(LIBLISA): $(LISAOBJECTS)
	$(ECHO) -n Linking $(LIBLISA)...
	$(AR) r $(LIBLISA) $(LISAOBJECTS)
	$(ECHO) done.

$(DESIGNEREXE): $(DLLNAME) $(LIBNAME) $(DESIGNEROBJECTS)
	$(ECHO) -n Linking $(DESIGNEREXE)...
	$(EXELINKER) $(DESIGNEROBJECTS) $(EXELINKER_OPTS) -o$(DESIGNEREXE)
	$(STRIP) $(STRIP_OPTS) $(DESIGNEREXE)
	$(PACKER) $(PACKER_OPTS) $(DESIGNEREXE) > NULL
	$(ECHO) done.

$(OBJTESTEXE): $(DLLNAME) $(LIBNAME) $(OBJTESTOBJECTS)
	$(ECHO) -n Linking $(OBJTESTEXE)...
	$(EXELINKER) $(OBJTESTOBJECTS) $(EXELINKER_OPTS) -o$(OBJTESTEXE)
	$(STRIP) $(STRIP_OPTS) $(OBJTESTEXE)
	$(PACKER) $(PACKER_OPTS) $(OBJTESTEXE) > NULL
	$(ECHO) done.

$(SAMPLE4DLL): $(SAMPLE4DLLOBJECTS)
	$(ECHO) Linking $(SAMPLE4DLL)...
	$(LINKER) $(SAMPLE4DLLOBJECTS) $(PLUGIN_LINKER_OPTS) $(SAMPLESLIBS) > NULL
	$(STRIP) $(STRIP_OPTS) $(SAMPLE4DLL)
	$(STRIP) $(LIBSTRIP_OPTS) $(SAMPLE4LIB)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE4DLL) > NULL
	$(ECHO) done.

$(SAMPLE1EXE): $(DLLNAME) $(LIBNAME) $(SAMPLE1OBJECTS)
	$(ECHO) -n Linking $(SAMPLE1EXE)...
	$(EXELINKER) $(SAMPLE1OBJECTS) $(EXELINKER_OPTS) -o$(SAMPLE1EXE)
	$(STRIP) $(STRIP_OPTS) $(SAMPLE1EXE)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE1EXE) > NULL
	$(ECHO) done.

$(SAMPLE2EXE): $(DLLNAME) $(LIBNAME) $(SAMPLE2OBJECTS)
	$(ECHO) -n Linking $(SAMPLE2EXE)...
	$(EXELINKER) $(SAMPLE2OBJECTS) $(EXELINKER_OPTS) -o$(SAMPLE2EXE)
	$(STRIP) $(STRIP_OPTS) $(SAMPLE2EXE)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE2EXE) > NULL
	$(ECHO) done.

$(SAMPLE3EXE): $(DLLNAME) $(LIBNAME) $(SAMPLE3OBJECTS)
	$(ECHO) -n Linking $(SAMPLE3EXE)...
	$(EXELINKER) $(SAMPLE3OBJECTS) $(EXELINKER_OPTS) -o$(SAMPLE3EXE)
	$(STRIP) $(STRIP_OPTS) $(SAMPLE3EXE)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE3EXE) > NULL
	$(ECHO) done.

$(SAMPLE4EXE): $(DLLNAME) $(LIBNAME) $(SAMPLE4DLL) $(SAMPLE4OBJECTS)
	$(ECHO) -n Linking $(SAMPLE4EXE)...
	$(EXELINKER) $(SAMPLE4OBJECTS) $(EXELINKER_OPTS) -L$(SAMPLE4DIR) -lplugin -o$(SAMPLE4EXE)
	$(STRIP) $(STRIP_OPTS) $(SAMPLE4EXE)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE4EXE) > NULL
	$(ECHO) done.

$(SAMPLE5EXE): $(DLLNAME) $(LIBNAME) $(SAMPLE5OBJECTS)
	$(ECHO) -n Linking $(SAMPLE5EXE)...
	$(EXELINKER) $(SAMPLE5OBJECTS) $(EXELINKER_OPTS) -o$(SAMPLE5EXE)
	$(STRIP) $(STRIP_OPTS) $(SAMPLE5EXE)
	$(PACKER) $(PACKER_OPTS) $(SAMPLE5EXE) > NULL
	$(ECHO) done.

$(TUTORIAL01EXE): $(DLLNAME) $(LIBNAME) $(TUTORIAL01OBJECTS)
	$(ECHO) -n Linking $(TUTORIAL01EXE)...
	$(EXELINKER) $(TUTORIAL01OBJECTS) $(EXELINKER_OPTS) -o$(TUTORIAL01EXE)
	$(STRIP) $(STRIP_OPTS) $(TUTORIAL01EXE)
	$(PACKER) $(PACKER_OPTS) $(TUTORIAL01EXE) > NULL
	$(ECHO) done.

$(TUTORIAL02EXE): $(DLLNAME) $(LIBNAME) $(TUTORIAL02OBJECTS)
	$(ECHO) -n Linking $(TUTORIAL02EXE)...
	$(EXELINKER) $(TUTORIAL02OBJECTS) $(EXELINKER_OPTS) -o$(TUTORIAL02EXE)
	$(STRIP) $(STRIP_OPTS) $(TUTORIAL02EXE)
	$(PACKER) $(PACKER_OPTS) $(TUTORIAL02EXE) > NULL
	$(ECHO) done.

include Makefile-objects

$(OBJECTDIR)/lisa_lisa.o: $(LISADIR)/lisa_win32.cpp
	$(ECHO) -n Compiling $(LISADIR)/lisa_win32.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISADIR)/lisa_win32.cpp -o $(OBJECTDIR)/lisa_lisa.o
	$(ECHO) done.
