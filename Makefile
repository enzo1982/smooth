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

OBJECTS = $(OBJECTDIR)/application.o $(OBJECTDIR)/background.o $(OBJECTDIR)/container.o $(OBJECTDIR)/containertype.o $(OBJECTDIR)/object.o $(OBJECTDIR)/objectmanager.o $(OBJECTDIR)/objectproperties.o $(OBJECTDIR)/objecttype.o $(OBJECTDIR)/toolwindow.o $(OBJECTDIR)/activearea.o $(OBJECTDIR)/arrows.o $(OBJECTDIR)/bitmap.o $(OBJECTDIR)/button.o $(OBJECTDIR)/checkbox.o $(OBJECTDIR)/editbox.o $(OBJECTDIR)/groupbox.o $(OBJECTDIR)/hyperlink.o $(OBJECTDIR)/layer.o $(OBJECTDIR)/optionbox.o $(OBJECTDIR)/progressbar.o $(OBJECTDIR)/scrollbar.o $(OBJECTDIR)/slider.o $(OBJECTDIR)/tabregister.o $(OBJECTDIR)/text.o $(OBJECTDIR)/combobox.o $(OBJECTDIR)/list.o $(OBJECTDIR)/listbox.o $(OBJECTDIR)/listentry.o $(OBJECTDIR)/mdiclient.o $(OBJECTDIR)/mdiwindow.o $(OBJECTDIR)/menu.o $(OBJECTDIR)/menubar.o $(OBJECTDIR)/menuentry.o $(OBJECTDIR)/popupmenu.o $(OBJECTDIR)/popupview.o $(OBJECTDIR)/date.o $(OBJECTDIR)/string.o $(OBJECTDIR)/surface.o $(OBJECTDIR)/surfacegdi.o $(OBJECTDIR)/time.o $(OBJECTDIR)/timer.o $(OBJECTDIR)/line.o $(OBJECTDIR)/point.o $(OBJECTDIR)/primitive.o $(OBJECTDIR)/rect.o $(OBJECTDIR)/size.o $(OBJECTDIR)/mutex.o $(OBJECTDIR)/semaphore.o $(OBJECTDIR)/thread.o $(OBJECTDIR)/threadmanager.o $(OBJECTDIR)/tree.o $(OBJECTDIR)/treeentry.o $(OBJECTDIR)/treeview.o $(OBJECTDIR)/client.o $(OBJECTDIR)/divisionbar.o $(OBJECTDIR)/drag.o $(OBJECTDIR)/statusbar.o $(OBJECTDIR)/titlebar.o $(OBJECTDIR)/tooltip.o $(OBJECTDIR)/window.o $(OBJECTDIR)/xml_attribute.o $(OBJECTDIR)/xml_document.o $(OBJECTDIR)/xml_node.o $(OBJECTDIR)/xml_parser.o $(OBJECTDIR)/colordlg.o $(OBJECTDIR)/dialogproperties.o $(OBJECTDIR)/dialogs.o $(OBJECTDIR)/dirdlg.o $(OBJECTDIR)/filedlg.o $(OBJECTDIR)/fontdlg.o $(OBJECTDIR)/afrikaans.o $(OBJECTDIR)/basque.o $(OBJECTDIR)/breton.o $(OBJECTDIR)/bulgarian.o $(OBJECTDIR)/catalan.o $(OBJECTDIR)/chinese.o $(OBJECTDIR)/chinese-simplified.o $(OBJECTDIR)/croatian.o $(OBJECTDIR)/czech.o $(OBJECTDIR)/danish.o $(OBJECTDIR)/dutch.o $(OBJECTDIR)/english.o $(OBJECTDIR)/esperanto.o $(OBJECTDIR)/estonian.o $(OBJECTDIR)/finnish.o $(OBJECTDIR)/french.o $(OBJECTDIR)/galician.o $(OBJECTDIR)/german.o $(OBJECTDIR)/greek.o $(OBJECTDIR)/hebrew.o $(OBJECTDIR)/hungarian.o $(OBJECTDIR)/icelandic.o $(OBJECTDIR)/irish.o $(OBJECTDIR)/italian.o $(OBJECTDIR)/japanese.o $(OBJECTDIR)/korean.o $(OBJECTDIR)/latin.o $(OBJECTDIR)/latvian.o $(OBJECTDIR)/lithuanian.o $(OBJECTDIR)/macedonian.o $(OBJECTDIR)/maori.o $(OBJECTDIR)/norwegian.o $(OBJECTDIR)/occitan.o $(OBJECTDIR)/polish.o $(OBJECTDIR)/portuguese.o $(OBJECTDIR)/portuguese-brazil.o $(OBJECTDIR)/romanian.o $(OBJECTDIR)/russian.o $(OBJECTDIR)/serbian.o $(OBJECTDIR)/slovak.o $(OBJECTDIR)/slovenian.o $(OBJECTDIR)/spanish.o $(OBJECTDIR)/swedish.o $(OBJECTDIR)/tamil.o $(OBJECTDIR)/thai.o $(OBJECTDIR)/turkish.o $(OBJECTDIR)/ukrainian.o $(OBJECTDIR)/walloon.o $(OBJECTDIR)/welsh.o $(OBJECTDIR)/i18n.o $(OBJECTDIR)/binary.o $(OBJECTDIR)/color.o $(OBJECTDIR)/definitions.o $(OBJECTDIR)/dllmain.o $(OBJECTDIR)/fstools.o $(OBJECTDIR)/loop.o $(OBJECTDIR)/mathtools.o $(OBJECTDIR)/messagebox.o $(OBJECTDIR)/metrics.o $(OBJECTDIR)/splashscreen.o $(OBJECTDIR)/stk.o $(OBJECTDIR)/toolkit.o
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

IOLIBOBJECTS = $(OBJECTDIR)/iolib_instream.o $(OBJECTDIR)/iolib_iolib.o $(OBJECTDIR)/iolib_iolibfilter.o $(OBJECTDIR)/iolib_iolibstream.o $(OBJECTDIR)/iolib_outstream.o $(OBJECTDIR)/iolib_socks4.o $(OBJECTDIR)/iolib_socks5.o
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
COMPILER_OPTS = -I$(INCLUDEDIR) -march=i586 -O6 -ffast-math -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -D__SMOOTH_DLL__ -DIOLIB_DLL -c
LINKER_OPTS = -L$(LIBDIR) -mwindows --output-lib $(LIBNAME) --output-exp $(EXPNAME) --output-def $(DEFNAME) -o $(DLLNAME)
PLUGIN_LINKER_OPTS = -L$(LIBDIR) -mwindows --output-lib $(SAMPLE4LIB) --output-exp $(SAMPLE4EXP) --output-def $(SAMPLE4DEF) -o $(SAMPLE4DLL)
EXECOMPILER_OPTS = -I$(INCLUDEDIR) -march=i586 -O6 -ffast-math -Wall -Wno-pmf-conversions -fno-exceptions -DUNICODE -D_UNICODE -c
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

$(OBJECTDIR)/application.o: $(BASICCLASSESDIR)/application.cpp $(SMOOTHINCLUDEDIR)/application.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/application.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/application.cpp -o $(OBJECTDIR)/application.o
	$(ECHO) done.

$(OBJECTDIR)/background.o: $(BASICCLASSESDIR)/background.cpp $(SMOOTHINCLUDEDIR)/background.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/background.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/background.cpp -o $(OBJECTDIR)/background.o
	$(ECHO) done.

$(OBJECTDIR)/container.o: $(BASICCLASSESDIR)/container.cpp $(SMOOTHINCLUDEDIR)/container.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/container.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/container.cpp -o $(OBJECTDIR)/container.o
	$(ECHO) done.

$(OBJECTDIR)/containertype.o: $(BASICCLASSESDIR)/containertype.cpp $(SMOOTHINCLUDEDIR)/containertype.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/containertype.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/containertype.cpp -o $(OBJECTDIR)/containertype.o
	$(ECHO) done.

$(OBJECTDIR)/object.o: $(BASICCLASSESDIR)/object.cpp $(SMOOTHINCLUDEDIR)/object.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/object.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/object.cpp -o $(OBJECTDIR)/object.o
	$(ECHO) done.

$(OBJECTDIR)/objectmanager.o: $(BASICCLASSESDIR)/objectmanager.cpp $(SMOOTHINCLUDEDIR)/objectmanager.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/objectmanager.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/objectmanager.cpp -o $(OBJECTDIR)/objectmanager.o
	$(ECHO) done.

$(OBJECTDIR)/objectproperties.o: $(BASICCLASSESDIR)/objectproperties.cpp $(SMOOTHINCLUDEDIR)/objectproperties.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/objectproperties.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/objectproperties.cpp -o $(OBJECTDIR)/objectproperties.o
	$(ECHO) done.

$(OBJECTDIR)/objecttype.o: $(BASICCLASSESDIR)/objecttype.cpp $(SMOOTHINCLUDEDIR)/objecttype.h $(SMOOTHINCLUDEDIR)/object.h
	$(ECHO) -n Compiling $(BASICCLASSESDIR)/objecttype.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(BASICCLASSESDIR)/objecttype.cpp -o $(OBJECTDIR)/objecttype.o
	$(ECHO) done.

$(OBJECTDIR)/toolwindow.o: $(INTERNALCLASSESDIR)/toolwindow.cpp $(SMOOTHINCLUDEDIR)/toolwindow.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(INTERNALCLASSESDIR)/toolwindow.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(INTERNALCLASSESDIR)/toolwindow.cpp -o $(OBJECTDIR)/toolwindow.o
	$(ECHO) done.

$(OBJECTDIR)/activearea.o: $(LAYERCLASSESDIR)/activearea.cpp $(SMOOTHINCLUDEDIR)/activearea.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/activearea.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/activearea.cpp -o $(OBJECTDIR)/activearea.o
	$(ECHO) done.

$(OBJECTDIR)/arrows.o: $(LAYERCLASSESDIR)/arrows.cpp $(SMOOTHINCLUDEDIR)/arrows.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/arrows.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/arrows.cpp -o $(OBJECTDIR)/arrows.o
	$(ECHO) done.

$(OBJECTDIR)/bitmap.o: $(LAYERCLASSESDIR)/bitmap.cpp $(SMOOTHINCLUDEDIR)/bitmap.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/bitmap.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/bitmap.cpp -o $(OBJECTDIR)/bitmap.o
	$(ECHO) done.

$(OBJECTDIR)/button.o: $(LAYERCLASSESDIR)/button.cpp $(SMOOTHINCLUDEDIR)/button.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/button.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/button.cpp -o $(OBJECTDIR)/button.o
	$(ECHO) done.

$(OBJECTDIR)/checkbox.o: $(LAYERCLASSESDIR)/checkbox.cpp $(SMOOTHINCLUDEDIR)/checkbox.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/checkbox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/checkbox.cpp -o $(OBJECTDIR)/checkbox.o
	$(ECHO) done.

$(OBJECTDIR)/editbox.o: $(LAYERCLASSESDIR)/editbox.cpp $(SMOOTHINCLUDEDIR)/editbox.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/editbox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/editbox.cpp -o $(OBJECTDIR)/editbox.o
	$(ECHO) done.

$(OBJECTDIR)/groupbox.o: $(LAYERCLASSESDIR)/groupbox.cpp $(SMOOTHINCLUDEDIR)/groupbox.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/groupbox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/groupbox.cpp -o $(OBJECTDIR)/groupbox.o
	$(ECHO) done.

$(OBJECTDIR)/hyperlink.o: $(LAYERCLASSESDIR)/hyperlink.cpp $(SMOOTHINCLUDEDIR)/hyperlink.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/hyperlink.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/hyperlink.cpp -o $(OBJECTDIR)/hyperlink.o
	$(ECHO) done.

$(OBJECTDIR)/layer.o: $(LAYERCLASSESDIR)/layer.cpp $(SMOOTHINCLUDEDIR)/layer.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/layer.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/layer.cpp -o $(OBJECTDIR)/layer.o
	$(ECHO) done.

$(OBJECTDIR)/optionbox.o: $(LAYERCLASSESDIR)/optionbox.cpp $(SMOOTHINCLUDEDIR)/optionbox.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/optionbox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/optionbox.cpp -o $(OBJECTDIR)/optionbox.o
	$(ECHO) done.

$(OBJECTDIR)/progressbar.o: $(LAYERCLASSESDIR)/progressbar.cpp $(SMOOTHINCLUDEDIR)/progressbar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/progressbar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/progressbar.cpp -o $(OBJECTDIR)/progressbar.o
	$(ECHO) done.

$(OBJECTDIR)/scrollbar.o: $(LAYERCLASSESDIR)/scrollbar.cpp $(SMOOTHINCLUDEDIR)/scrollbar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/scrollbar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/scrollbar.cpp -o $(OBJECTDIR)/scrollbar.o
	$(ECHO) done.

$(OBJECTDIR)/slider.o: $(LAYERCLASSESDIR)/slider.cpp $(SMOOTHINCLUDEDIR)/slider.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/slider.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/slider.cpp -o $(OBJECTDIR)/slider.o
	$(ECHO) done.

$(OBJECTDIR)/tabregister.o: $(LAYERCLASSESDIR)/tabregister.cpp $(SMOOTHINCLUDEDIR)/tabregister.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/tabregister.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/tabregister.cpp -o $(OBJECTDIR)/tabregister.o
	$(ECHO) done.

$(OBJECTDIR)/text.o: $(LAYERCLASSESDIR)/text.cpp $(SMOOTHINCLUDEDIR)/text.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LAYERCLASSESDIR)/text.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LAYERCLASSESDIR)/text.cpp -o $(OBJECTDIR)/text.o
	$(ECHO) done.

$(OBJECTDIR)/combobox.o: $(LISTCLASSESDIR)/combobox.cpp $(SMOOTHINCLUDEDIR)/combobox.h
	$(ECHO) -n Compiling $(LISTCLASSESDIR)/combobox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISTCLASSESDIR)/combobox.cpp -o $(OBJECTDIR)/combobox.o
	$(ECHO) done.

$(OBJECTDIR)/list.o: $(LISTCLASSESDIR)/list.cpp $(SMOOTHINCLUDEDIR)/list.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LISTCLASSESDIR)/list.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISTCLASSESDIR)/list.cpp -o $(OBJECTDIR)/list.o
	$(ECHO) done.

$(OBJECTDIR)/listbox.o: $(LISTCLASSESDIR)/listbox.cpp $(SMOOTHINCLUDEDIR)/listbox.h
	$(ECHO) -n Compiling $(LISTCLASSESDIR)/listbox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISTCLASSESDIR)/listbox.cpp -o $(OBJECTDIR)/listbox.o
	$(ECHO) done.

$(OBJECTDIR)/listentry.o: $(LISTCLASSESDIR)/listentry.cpp $(SMOOTHINCLUDEDIR)/list.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(LISTCLASSESDIR)/listentry.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISTCLASSESDIR)/listentry.cpp -o $(OBJECTDIR)/listentry.o
	$(ECHO) done.

$(OBJECTDIR)/mdiclient.o: $(MDICLASSESDIR)/mdiclient.cpp $(SMOOTHINCLUDEDIR)/mdiclient.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MDICLASSESDIR)/mdiclient.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MDICLASSESDIR)/mdiclient.cpp -o $(OBJECTDIR)/mdiclient.o
	$(ECHO) done.

$(OBJECTDIR)/mdiwindow.o: $(MDICLASSESDIR)/mdiwindow.cpp $(SMOOTHINCLUDEDIR)/mdiwindow.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MDICLASSESDIR)/mdiwindow.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MDICLASSESDIR)/mdiwindow.cpp -o $(OBJECTDIR)/mdiwindow.o
	$(ECHO) done.

$(OBJECTDIR)/menu.o: $(MENUCLASSESDIR)/menu.cpp $(SMOOTHINCLUDEDIR)/menu.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MENUCLASSESDIR)/menu.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MENUCLASSESDIR)/menu.cpp -o $(OBJECTDIR)/menu.o
	$(ECHO) done.

$(OBJECTDIR)/menubar.o: $(MENUCLASSESDIR)/menubar.cpp $(SMOOTHINCLUDEDIR)/menubar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MENUCLASSESDIR)/menubar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MENUCLASSESDIR)/menubar.cpp -o $(OBJECTDIR)/menubar.o
	$(ECHO) done.

$(OBJECTDIR)/menuentry.o: $(MENUCLASSESDIR)/menuentry.cpp $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MENUCLASSESDIR)/menuentry.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MENUCLASSESDIR)/menuentry.cpp -o $(OBJECTDIR)/menuentry.o
	$(ECHO) done.

$(OBJECTDIR)/popupmenu.o: $(MENUCLASSESDIR)/popupmenu.cpp $(SMOOTHINCLUDEDIR)/popupmenu.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MENUCLASSESDIR)/popupmenu.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MENUCLASSESDIR)/popupmenu.cpp -o $(OBJECTDIR)/popupmenu.o
	$(ECHO) done.

$(OBJECTDIR)/popupview.o: $(MENUCLASSESDIR)/popupview.cpp $(SMOOTHINCLUDEDIR)/popupview.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MENUCLASSESDIR)/popupview.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MENUCLASSESDIR)/popupview.cpp -o $(OBJECTDIR)/popupview.o
	$(ECHO) done.

$(OBJECTDIR)/date.o: $(MISCCLASSESDIR)/date.cpp $(SMOOTHINCLUDEDIR)/date.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/date.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/date.cpp -o $(OBJECTDIR)/date.o
	$(ECHO) done.

$(OBJECTDIR)/string.o: $(MISCCLASSESDIR)/string.cpp $(SMOOTHINCLUDEDIR)/string.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/string.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/string.cpp -o $(OBJECTDIR)/string.o
	$(ECHO) done.

$(OBJECTDIR)/surface.o: $(MISCCLASSESDIR)/surface.cpp $(SMOOTHINCLUDEDIR)/surface.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/surface.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/surface.cpp -o $(OBJECTDIR)/surface.o
	$(ECHO) done.

$(OBJECTDIR)/surfacegdi.o: $(MISCCLASSESDIR)/surfacegdi.cpp $(SMOOTHINCLUDEDIR)/surfacegdi.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/surfacegdi.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/surfacegdi.cpp -o $(OBJECTDIR)/surfacegdi.o
	$(ECHO) done.

$(OBJECTDIR)/time.o: $(MISCCLASSESDIR)/time.cpp $(SMOOTHINCLUDEDIR)/time.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/time.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/time.cpp -o $(OBJECTDIR)/time.o
	$(ECHO) done.

$(OBJECTDIR)/timer.o: $(MISCCLASSESDIR)/timer.cpp $(SMOOTHINCLUDEDIR)/timer.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MISCCLASSESDIR)/timer.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCCLASSESDIR)/timer.cpp -o $(OBJECTDIR)/timer.o
	$(ECHO) done.

$(OBJECTDIR)/line.o: $(PRIMITIVESCLASSESDIR)/line.cpp $(SMOOTHINCLUDEDIR)/line.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(PRIMITIVESCLASSESDIR)/line.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PRIMITIVESCLASSESDIR)/line.cpp -o $(OBJECTDIR)/line.o
	$(ECHO) done.

$(OBJECTDIR)/point.o: $(PRIMITIVESCLASSESDIR)/point.cpp $(SMOOTHINCLUDEDIR)/point.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(PRIMITIVESCLASSESDIR)/point.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PRIMITIVESCLASSESDIR)/point.cpp -o $(OBJECTDIR)/point.o
	$(ECHO) done.

$(OBJECTDIR)/primitive.o: $(PRIMITIVESCLASSESDIR)/primitive.cpp $(SMOOTHINCLUDEDIR)/primitive.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(PRIMITIVESCLASSESDIR)/primitive.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PRIMITIVESCLASSESDIR)/primitive.cpp -o $(OBJECTDIR)/primitive.o
	$(ECHO) done.

$(OBJECTDIR)/rect.o: $(PRIMITIVESCLASSESDIR)/rect.cpp $(SMOOTHINCLUDEDIR)/rect.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(PRIMITIVESCLASSESDIR)/rect.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PRIMITIVESCLASSESDIR)/rect.cpp -o $(OBJECTDIR)/rect.o
	$(ECHO) done.

$(OBJECTDIR)/size.o: $(PRIMITIVESCLASSESDIR)/size.cpp $(SMOOTHINCLUDEDIR)/size.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(PRIMITIVESCLASSESDIR)/size.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PRIMITIVESCLASSESDIR)/size.cpp -o $(OBJECTDIR)/size.o
	$(ECHO) done.

$(OBJECTDIR)/mutex.o: $(THREADSCLASSESDIR)/mutex.cpp $(SMOOTHINCLUDEDIR)/mutex.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(THREADSCLASSESDIR)/mutex.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(THREADSCLASSESDIR)/mutex.cpp -o $(OBJECTDIR)/mutex.o
	$(ECHO) done.

$(OBJECTDIR)/semaphore.o: $(THREADSCLASSESDIR)/semaphore.cpp $(SMOOTHINCLUDEDIR)/semaphore.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(THREADSCLASSESDIR)/semaphore.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(THREADSCLASSESDIR)/semaphore.cpp -o $(OBJECTDIR)/semaphore.o
	$(ECHO) done.

$(OBJECTDIR)/thread.o: $(THREADSCLASSESDIR)/thread.cpp $(SMOOTHINCLUDEDIR)/thread.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(THREADSCLASSESDIR)/thread.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(THREADSCLASSESDIR)/thread.cpp -o $(OBJECTDIR)/thread.o
	$(ECHO) done.

$(OBJECTDIR)/threadmanager.o: $(THREADSCLASSESDIR)/threadmanager.cpp $(SMOOTHINCLUDEDIR)/threadmanager.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(THREADSCLASSESDIR)/threadmanager.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(THREADSCLASSESDIR)/threadmanager.cpp -o $(OBJECTDIR)/threadmanager.o
	$(ECHO) done.

$(OBJECTDIR)/tree.o: $(TREECLASSESDIR)/tree.cpp $(SMOOTHINCLUDEDIR)/tree.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(TREECLASSESDIR)/tree.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(TREECLASSESDIR)/tree.cpp -o $(OBJECTDIR)/tree.o
	$(ECHO) done.

$(OBJECTDIR)/treeentry.o: $(TREECLASSESDIR)/treeentry.cpp $(SMOOTHINCLUDEDIR)/tree.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(TREECLASSESDIR)/treeentry.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(TREECLASSESDIR)/treeentry.cpp -o $(OBJECTDIR)/treeentry.o
	$(ECHO) done.

$(OBJECTDIR)/treeview.o: $(TREECLASSESDIR)/treeview.cpp $(SMOOTHINCLUDEDIR)/treeview.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(TREECLASSESDIR)/treeview.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(TREECLASSESDIR)/treeview.cpp -o $(OBJECTDIR)/treeview.o
	$(ECHO) done.

$(OBJECTDIR)/client.o: $(WINDOWCLASSESDIR)/client.cpp $(SMOOTHINCLUDEDIR)/client.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/client.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/client.cpp -o $(OBJECTDIR)/client.o
	$(ECHO) done.

$(OBJECTDIR)/divisionbar.o: $(WINDOWCLASSESDIR)/divisionbar.cpp $(SMOOTHINCLUDEDIR)/divisionbar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/divisionbar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/divisionbar.cpp -o $(OBJECTDIR)/divisionbar.o
	$(ECHO) done.

$(OBJECTDIR)/drag.o: $(WINDOWCLASSESDIR)/drag.cpp $(SMOOTHINCLUDEDIR)/drag.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/drag.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/drag.cpp -o $(OBJECTDIR)/drag.o
	$(ECHO) done.

$(OBJECTDIR)/statusbar.o: $(WINDOWCLASSESDIR)/statusbar.cpp $(SMOOTHINCLUDEDIR)/statusbar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/statusbar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/statusbar.cpp -o $(OBJECTDIR)/statusbar.o
	$(ECHO) done.

$(OBJECTDIR)/titlebar.o: $(WINDOWCLASSESDIR)/titlebar.cpp $(SMOOTHINCLUDEDIR)/titlebar.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/titlebar.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/titlebar.cpp -o $(OBJECTDIR)/titlebar.o
	$(ECHO) done.

$(OBJECTDIR)/tooltip.o: $(WINDOWCLASSESDIR)/tooltip.cpp $(SMOOTHINCLUDEDIR)/tooltip.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/tooltip.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/tooltip.cpp -o $(OBJECTDIR)/tooltip.o
	$(ECHO) done.

$(OBJECTDIR)/window.o: $(WINDOWCLASSESDIR)/window.cpp $(SMOOTHINCLUDEDIR)/window.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(WINDOWCLASSESDIR)/window.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(WINDOWCLASSESDIR)/window.cpp -o $(OBJECTDIR)/window.o
	$(ECHO) done.

$(OBJECTDIR)/xml_attribute.o: $(XMLCLASSESDIR)/attribute.cpp $(SMOOTHINCLUDEDIR)/xml/attribute.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(XMLCLASSESDIR)/attribute.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(XMLCLASSESDIR)/attribute.cpp -o $(OBJECTDIR)/xml_attribute.o
	$(ECHO) done.

$(OBJECTDIR)/xml_document.o: $(XMLCLASSESDIR)/document.cpp $(SMOOTHINCLUDEDIR)/xml/document.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(XMLCLASSESDIR)/document.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(XMLCLASSESDIR)/document.cpp -o $(OBJECTDIR)/xml_document.o
	$(ECHO) done.

$(OBJECTDIR)/xml_node.o: $(XMLCLASSESDIR)/node.cpp $(SMOOTHINCLUDEDIR)/xml/node.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(XMLCLASSESDIR)/node.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(XMLCLASSESDIR)/node.cpp -o $(OBJECTDIR)/xml_node.o
	$(ECHO) done.

$(OBJECTDIR)/xml_parser.o: $(XMLCLASSESDIR)/parser.cpp $(SMOOTHINCLUDEDIR)/xml/parser.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(XMLCLASSESDIR)/parser.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(XMLCLASSESDIR)/parser.cpp -o $(OBJECTDIR)/xml_parser.o
	$(ECHO) done.

$(OBJECTDIR)/colordlg.o: $(DIALOGSDIR)/colordlg.cpp $(SMOOTHINCLUDEDIR)/colordlg.h $(SMOOTHINCLUDEDIR)/definitions.h $(SMOOTHINCLUDEDIR)/color.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/colordlg.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/colordlg.cpp -o $(OBJECTDIR)/colordlg.o
	$(ECHO) done.

$(OBJECTDIR)/dialogproperties.o: $(DIALOGSDIR)/dialogproperties.cpp $(SMOOTHINCLUDEDIR)/dialogproperties.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/dialogproperties.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/dialogproperties.cpp -o $(OBJECTDIR)/dialogproperties.o
	$(ECHO) done.

$(OBJECTDIR)/dialogs.o: $(DIALOGSDIR)/dialogs.cpp $(SMOOTHINCLUDEDIR)/dialogs.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/dialogs.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/dialogs.cpp -o $(OBJECTDIR)/dialogs.o
	$(ECHO) done.

$(OBJECTDIR)/dirdlg.o: $(DIALOGSDIR)/dirdlg.cpp $(SMOOTHINCLUDEDIR)/dirdlg.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/dirdlg.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/dirdlg.cpp -o $(OBJECTDIR)/dirdlg.o
	$(ECHO) done.

$(OBJECTDIR)/filedlg.o: $(DIALOGSDIR)/filedlg.cpp $(SMOOTHINCLUDEDIR)/filedlg.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/filedlg.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/filedlg.cpp -o $(OBJECTDIR)/filedlg.o
	$(ECHO) done.

$(OBJECTDIR)/fontdlg.o: $(DIALOGSDIR)/fontdlg.cpp $(SMOOTHINCLUDEDIR)/fontdlg.h $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(DIALOGSDIR)/fontdlg.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(DIALOGSDIR)/fontdlg.cpp -o $(OBJECTDIR)/fontdlg.o
	$(ECHO) done.

$(OBJECTDIR)/afrikaans.o: $(I18NDIR)/afrikaans.cpp $(SMOOTHINCLUDEDIR)/i18n/afrikaans.h
	$(ECHO) -n Compiling $(I18NDIR)/afrikaans.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/afrikaans.cpp -o $(OBJECTDIR)/afrikaans.o
	$(ECHO) done.

$(OBJECTDIR)/basque.o: $(I18NDIR)/basque.cpp $(SMOOTHINCLUDEDIR)/i18n/basque.h
	$(ECHO) -n Compiling $(I18NDIR)/basque.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/basque.cpp -o $(OBJECTDIR)/basque.o
	$(ECHO) done.

$(OBJECTDIR)/breton.o: $(I18NDIR)/breton.cpp $(SMOOTHINCLUDEDIR)/i18n/breton.h
	$(ECHO) -n Compiling $(I18NDIR)/breton.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/breton.cpp -o $(OBJECTDIR)/breton.o
	$(ECHO) done.

$(OBJECTDIR)/bulgarian.o: $(I18NDIR)/bulgarian.cpp $(SMOOTHINCLUDEDIR)/i18n/bulgarian.h
	$(ECHO) -n Compiling $(I18NDIR)/bulgarian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/bulgarian.cpp -o $(OBJECTDIR)/bulgarian.o
	$(ECHO) done.

$(OBJECTDIR)/catalan.o: $(I18NDIR)/catalan.cpp $(SMOOTHINCLUDEDIR)/i18n/catalan.h
	$(ECHO) -n Compiling $(I18NDIR)/catalan.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/catalan.cpp -o $(OBJECTDIR)/catalan.o
	$(ECHO) done.

$(OBJECTDIR)/chinese.o: $(I18NDIR)/chinese.cpp $(SMOOTHINCLUDEDIR)/i18n/chinese.h
	$(ECHO) -n Compiling $(I18NDIR)/chinese.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/chinese.cpp -o $(OBJECTDIR)/chinese.o
	$(ECHO) done.

$(OBJECTDIR)/chinese-simplified.o: $(I18NDIR)/chinese-simplified.cpp $(SMOOTHINCLUDEDIR)/i18n/chinese-simplified.h
	$(ECHO) -n Compiling $(I18NDIR)/chinese-simplified.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/chinese-simplified.cpp -o $(OBJECTDIR)/chinese-simplified.o
	$(ECHO) done.

$(OBJECTDIR)/croatian.o: $(I18NDIR)/croatian.cpp $(SMOOTHINCLUDEDIR)/i18n/croatian.h
	$(ECHO) -n Compiling $(I18NDIR)/croatian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/croatian.cpp -o $(OBJECTDIR)/croatian.o
	$(ECHO) done.

$(OBJECTDIR)/czech.o: $(I18NDIR)/czech.cpp $(SMOOTHINCLUDEDIR)/i18n/czech.h
	$(ECHO) -n Compiling $(I18NDIR)/czech.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/czech.cpp -o $(OBJECTDIR)/czech.o
	$(ECHO) done.

$(OBJECTDIR)/danish.o: $(I18NDIR)/danish.cpp $(SMOOTHINCLUDEDIR)/i18n/danish.h
	$(ECHO) -n Compiling $(I18NDIR)/danish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/danish.cpp -o $(OBJECTDIR)/danish.o
	$(ECHO) done.

$(OBJECTDIR)/dutch.o: $(I18NDIR)/dutch.cpp $(SMOOTHINCLUDEDIR)/i18n/dutch.h
	$(ECHO) -n Compiling $(I18NDIR)/dutch.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/dutch.cpp -o $(OBJECTDIR)/dutch.o
	$(ECHO) done.

$(OBJECTDIR)/english.o: $(I18NDIR)/english.cpp $(SMOOTHINCLUDEDIR)/i18n/english.h
	$(ECHO) -n Compiling $(I18NDIR)/english.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/english.cpp -o $(OBJECTDIR)/english.o
	$(ECHO) done.

$(OBJECTDIR)/esperanto.o: $(I18NDIR)/esperanto.cpp $(SMOOTHINCLUDEDIR)/i18n/esperanto.h
	$(ECHO) -n Compiling $(I18NDIR)/esperanto.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/esperanto.cpp -o $(OBJECTDIR)/esperanto.o
	$(ECHO) done.

$(OBJECTDIR)/estonian.o: $(I18NDIR)/estonian.cpp $(SMOOTHINCLUDEDIR)/i18n/estonian.h
	$(ECHO) -n Compiling $(I18NDIR)/estonian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/estonian.cpp -o $(OBJECTDIR)/estonian.o
	$(ECHO) done.

$(OBJECTDIR)/finnish.o: $(I18NDIR)/finnish.cpp $(SMOOTHINCLUDEDIR)/i18n/finnish.h
	$(ECHO) -n Compiling $(I18NDIR)/finnish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/finnish.cpp -o $(OBJECTDIR)/finnish.o
	$(ECHO) done.

$(OBJECTDIR)/french.o: $(I18NDIR)/french.cpp $(SMOOTHINCLUDEDIR)/i18n/french.h
	$(ECHO) -n Compiling $(I18NDIR)/french.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/french.cpp -o $(OBJECTDIR)/french.o
	$(ECHO) done.

$(OBJECTDIR)/galician.o: $(I18NDIR)/galician.cpp $(SMOOTHINCLUDEDIR)/i18n/galician.h
	$(ECHO) -n Compiling $(I18NDIR)/galician.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/galician.cpp -o $(OBJECTDIR)/galician.o
	$(ECHO) done.

$(OBJECTDIR)/german.o: $(I18NDIR)/german.cpp $(SMOOTHINCLUDEDIR)/i18n/german.h
	$(ECHO) -n Compiling $(I18NDIR)/german.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/german.cpp -o $(OBJECTDIR)/german.o
	$(ECHO) done.

$(OBJECTDIR)/greek.o: $(I18NDIR)/greek.cpp $(SMOOTHINCLUDEDIR)/i18n/greek.h
	$(ECHO) -n Compiling $(I18NDIR)/greek.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/greek.cpp -o $(OBJECTDIR)/greek.o
	$(ECHO) done.

$(OBJECTDIR)/hebrew.o: $(I18NDIR)/hebrew.cpp $(SMOOTHINCLUDEDIR)/i18n/hebrew.h
	$(ECHO) -n Compiling $(I18NDIR)/hebrew.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/hebrew.cpp -o $(OBJECTDIR)/hebrew.o
	$(ECHO) done.

$(OBJECTDIR)/hungarian.o: $(I18NDIR)/hungarian.cpp $(SMOOTHINCLUDEDIR)/i18n/hungarian.h
	$(ECHO) -n Compiling $(I18NDIR)/hungarian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/hungarian.cpp -o $(OBJECTDIR)/hungarian.o
	$(ECHO) done.

$(OBJECTDIR)/i18n.o: $(I18NDIR)/i18n.cpp $(SMOOTHINCLUDEDIR)/i18n.h
	$(ECHO) -n Compiling $(I18NDIR)/i18n.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/i18n.cpp -o $(OBJECTDIR)/i18n.o
	$(ECHO) done.

$(OBJECTDIR)/icelandic.o: $(I18NDIR)/icelandic.cpp $(SMOOTHINCLUDEDIR)/i18n/icelandic.h
	$(ECHO) -n Compiling $(I18NDIR)/icelandic.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/icelandic.cpp -o $(OBJECTDIR)/icelandic.o
	$(ECHO) done.

$(OBJECTDIR)/irish.o: $(I18NDIR)/irish.cpp $(SMOOTHINCLUDEDIR)/i18n/irish.h
	$(ECHO) -n Compiling $(I18NDIR)/irish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/irish.cpp -o $(OBJECTDIR)/irish.o
	$(ECHO) done.

$(OBJECTDIR)/italian.o: $(I18NDIR)/italian.cpp $(SMOOTHINCLUDEDIR)/i18n/italian.h
	$(ECHO) -n Compiling $(I18NDIR)/italian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/italian.cpp -o $(OBJECTDIR)/italian.o
	$(ECHO) done.

$(OBJECTDIR)/japanese.o: $(I18NDIR)/japanese.cpp $(SMOOTHINCLUDEDIR)/i18n/japanese.h
	$(ECHO) -n Compiling $(I18NDIR)/japanese.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/japanese.cpp -o $(OBJECTDIR)/japanese.o
	$(ECHO) done.

$(OBJECTDIR)/korean.o: $(I18NDIR)/korean.cpp $(SMOOTHINCLUDEDIR)/i18n/korean.h
	$(ECHO) -n Compiling $(I18NDIR)/korean.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/korean.cpp -o $(OBJECTDIR)/korean.o
	$(ECHO) done.

$(OBJECTDIR)/latin.o: $(I18NDIR)/latin.cpp $(SMOOTHINCLUDEDIR)/i18n/latin.h
	$(ECHO) -n Compiling $(I18NDIR)/latin.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/latin.cpp -o $(OBJECTDIR)/latin.o
	$(ECHO) done.

$(OBJECTDIR)/latvian.o: $(I18NDIR)/latvian.cpp $(SMOOTHINCLUDEDIR)/i18n/latvian.h
	$(ECHO) -n Compiling $(I18NDIR)/latvian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/latvian.cpp -o $(OBJECTDIR)/latvian.o
	$(ECHO) done.

$(OBJECTDIR)/lithuanian.o: $(I18NDIR)/lithuanian.cpp $(SMOOTHINCLUDEDIR)/i18n/lithuanian.h
	$(ECHO) -n Compiling $(I18NDIR)/lithuanian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/lithuanian.cpp -o $(OBJECTDIR)/lithuanian.o
	$(ECHO) done.

$(OBJECTDIR)/macedonian.o: $(I18NDIR)/macedonian.cpp $(SMOOTHINCLUDEDIR)/i18n/macedonian.h
	$(ECHO) -n Compiling $(I18NDIR)/macedonian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/macedonian.cpp -o $(OBJECTDIR)/macedonian.o
	$(ECHO) done.

$(OBJECTDIR)/maori.o: $(I18NDIR)/maori.cpp $(SMOOTHINCLUDEDIR)/i18n/maori.h
	$(ECHO) -n Compiling $(I18NDIR)/maori.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/maori.cpp -o $(OBJECTDIR)/maori.o
	$(ECHO) done.

$(OBJECTDIR)/norwegian.o: $(I18NDIR)/norwegian.cpp $(SMOOTHINCLUDEDIR)/i18n/norwegian.h
	$(ECHO) -n Compiling $(I18NDIR)/norwegian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/norwegian.cpp -o $(OBJECTDIR)/norwegian.o
	$(ECHO) done.

$(OBJECTDIR)/occitan.o: $(I18NDIR)/occitan.cpp $(SMOOTHINCLUDEDIR)/i18n/occitan.h
	$(ECHO) -n Compiling $(I18NDIR)/occitan.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/occitan.cpp -o $(OBJECTDIR)/occitan.o
	$(ECHO) done.

$(OBJECTDIR)/polish.o: $(I18NDIR)/polish.cpp $(SMOOTHINCLUDEDIR)/i18n/polish.h
	$(ECHO) -n Compiling $(I18NDIR)/polish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/polish.cpp -o $(OBJECTDIR)/polish.o
	$(ECHO) done.

$(OBJECTDIR)/portuguese.o: $(I18NDIR)/portuguese.cpp $(SMOOTHINCLUDEDIR)/i18n/portuguese.h
	$(ECHO) -n Compiling $(I18NDIR)/portuguese.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/portuguese.cpp -o $(OBJECTDIR)/portuguese.o
	$(ECHO) done.

$(OBJECTDIR)/portuguese-brazil.o: $(I18NDIR)/portuguese-brazil.cpp $(SMOOTHINCLUDEDIR)/i18n/portuguese-brazil.h
	$(ECHO) -n Compiling $(I18NDIR)/portuguese-brazil.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/portuguese-brazil.cpp -o $(OBJECTDIR)/portuguese-brazil.o
	$(ECHO) done.

$(OBJECTDIR)/romanian.o: $(I18NDIR)/romanian.cpp $(SMOOTHINCLUDEDIR)/i18n/romanian.h
	$(ECHO) -n Compiling $(I18NDIR)/romanian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/romanian.cpp -o $(OBJECTDIR)/romanian.o
	$(ECHO) done.

$(OBJECTDIR)/russian.o: $(I18NDIR)/russian.cpp $(SMOOTHINCLUDEDIR)/i18n/russian.h
	$(ECHO) -n Compiling $(I18NDIR)/russian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/russian.cpp -o $(OBJECTDIR)/russian.o
	$(ECHO) done.

$(OBJECTDIR)/serbian.o: $(I18NDIR)/serbian.cpp $(SMOOTHINCLUDEDIR)/i18n/serbian.h
	$(ECHO) -n Compiling $(I18NDIR)/serbian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/serbian.cpp -o $(OBJECTDIR)/serbian.o
	$(ECHO) done.

$(OBJECTDIR)/slovak.o: $(I18NDIR)/slovak.cpp $(SMOOTHINCLUDEDIR)/i18n/slovak.h
	$(ECHO) -n Compiling $(I18NDIR)/slovak.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/slovak.cpp -o $(OBJECTDIR)/slovak.o
	$(ECHO) done.

$(OBJECTDIR)/slovenian.o: $(I18NDIR)/slovenian.cpp $(SMOOTHINCLUDEDIR)/i18n/slovenian.h
	$(ECHO) -n Compiling $(I18NDIR)/slovenian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/slovenian.cpp -o $(OBJECTDIR)/slovenian.o
	$(ECHO) done.

$(OBJECTDIR)/spanish.o: $(I18NDIR)/spanish.cpp $(SMOOTHINCLUDEDIR)/i18n/spanish.h
	$(ECHO) -n Compiling $(I18NDIR)/spanish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/spanish.cpp -o $(OBJECTDIR)/spanish.o
	$(ECHO) done.

$(OBJECTDIR)/swedish.o: $(I18NDIR)/swedish.cpp $(SMOOTHINCLUDEDIR)/i18n/swedish.h
	$(ECHO) -n Compiling $(I18NDIR)/swedish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/swedish.cpp -o $(OBJECTDIR)/swedish.o
	$(ECHO) done.

$(OBJECTDIR)/tamil.o: $(I18NDIR)/tamil.cpp $(SMOOTHINCLUDEDIR)/i18n/tamil.h
	$(ECHO) -n Compiling $(I18NDIR)/tamil.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/tamil.cpp -o $(OBJECTDIR)/tamil.o
	$(ECHO) done.

$(OBJECTDIR)/thai.o: $(I18NDIR)/thai.cpp $(SMOOTHINCLUDEDIR)/i18n/thai.h
	$(ECHO) -n Compiling $(I18NDIR)/thai.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/thai.cpp -o $(OBJECTDIR)/thai.o
	$(ECHO) done.

$(OBJECTDIR)/turkish.o: $(I18NDIR)/turkish.cpp $(SMOOTHINCLUDEDIR)/i18n/turkish.h
	$(ECHO) -n Compiling $(I18NDIR)/turkish.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/turkish.cpp -o $(OBJECTDIR)/turkish.o
	$(ECHO) done.

$(OBJECTDIR)/ukrainian.o: $(I18NDIR)/ukrainian.cpp $(SMOOTHINCLUDEDIR)/i18n/ukrainian.h
	$(ECHO) -n Compiling $(I18NDIR)/ukrainian.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/ukrainian.cpp -o $(OBJECTDIR)/ukrainian.o
	$(ECHO) done.

$(OBJECTDIR)/walloon.o: $(I18NDIR)/walloon.cpp $(SMOOTHINCLUDEDIR)/i18n/walloon.h
	$(ECHO) -n Compiling $(I18NDIR)/walloon.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/walloon.cpp -o $(OBJECTDIR)/walloon.o
	$(ECHO) done.

$(OBJECTDIR)/welsh.o: $(I18NDIR)/welsh.cpp $(SMOOTHINCLUDEDIR)/i18n/welsh.h
	$(ECHO) -n Compiling $(I18NDIR)/welsh.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(I18NDIR)/welsh.cpp -o $(OBJECTDIR)/welsh.o
	$(ECHO) done.

$(OBJECTDIR)/binary.o: $(MISCDIR)/binary.cpp $(SMOOTHINCLUDEDIR)/binary.h
	$(ECHO) -n Compiling $(MISCDIR)/binary.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/binary.cpp -o $(OBJECTDIR)/binary.o
	$(ECHO) done.

$(OBJECTDIR)/color.o: $(MISCDIR)/color.cpp $(SMOOTHINCLUDEDIR)/color.h
	$(ECHO) -n Compiling $(MISCDIR)/color.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/color.cpp -o $(OBJECTDIR)/color.o
	$(ECHO) done.

$(OBJECTDIR)/definitions.o: $(MISCDIR)/definitions.cpp $(SMOOTHINCLUDEDIR)/definitions.h
	$(ECHO) -n Compiling $(MISCDIR)/definitions.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/definitions.cpp -o $(OBJECTDIR)/definitions.o
	$(ECHO) done.

$(OBJECTDIR)/dllmain.o: $(MISCDIR)/dllmain.cpp $(SMOOTHINCLUDEDIR)/dllmain.h
	$(ECHO) -n Compiling $(MISCDIR)/dllmain.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/dllmain.cpp -o $(OBJECTDIR)/dllmain.o
	$(ECHO) done.

$(OBJECTDIR)/fstools.o: $(MISCDIR)/fstools.cpp $(SMOOTHINCLUDEDIR)/fstools.h
	$(ECHO) -n Compiling $(MISCDIR)/fstools.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/fstools.cpp -o $(OBJECTDIR)/fstools.o
	$(ECHO) done.

$(OBJECTDIR)/loop.o: $(MISCDIR)/loop.cpp $(SMOOTHINCLUDEDIR)/loop.h
	$(ECHO) -n Compiling $(MISCDIR)/loop.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/loop.cpp -o $(OBJECTDIR)/loop.o
	$(ECHO) done.

$(OBJECTDIR)/mathtools.o: $(MISCDIR)/mathtools.cpp $(SMOOTHINCLUDEDIR)/mathtools.h
	$(ECHO) -n Compiling $(MISCDIR)/mathtools.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/mathtools.cpp -o $(OBJECTDIR)/mathtools.o
	$(ECHO) done.

$(OBJECTDIR)/messagebox.o: $(MISCDIR)/messagebox.cpp $(SMOOTHINCLUDEDIR)/messagebox.h
	$(ECHO) -n Compiling $(MISCDIR)/messagebox.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/messagebox.cpp -o $(OBJECTDIR)/messagebox.o
	$(ECHO) done.

$(OBJECTDIR)/metrics.o: $(MISCDIR)/metrics.cpp $(SMOOTHINCLUDEDIR)/metrics.h
	$(ECHO) -n Compiling $(MISCDIR)/metrics.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/metrics.cpp -o $(OBJECTDIR)/metrics.o
	$(ECHO) done.

$(OBJECTDIR)/splashscreen.o: $(MISCDIR)/splashscreen.cpp $(SMOOTHINCLUDEDIR)/splashscreen.h
	$(ECHO) -n Compiling $(MISCDIR)/splashscreen.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/splashscreen.cpp -o $(OBJECTDIR)/splashscreen.o
	$(ECHO) done.

$(OBJECTDIR)/stk.o: $(MISCDIR)/stk.cpp $(SMOOTHINCLUDEDIR)/stk.h $(SMOOTHINCLUDEDIR)/version.h
	$(ECHO) -n Compiling $(MISCDIR)/stk.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/stk.cpp -o $(OBJECTDIR)/stk.o
	$(ECHO) done.

$(OBJECTDIR)/toolkit.o: $(MISCDIR)/toolkit.cpp $(SMOOTHINCLUDEDIR)/toolkit.h
	$(ECHO) -n Compiling $(MISCDIR)/toolkit.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(MISCDIR)/toolkit.cpp -o $(OBJECTDIR)/toolkit.o
	$(ECHO) done.

$(OBJECTDIR)/resources.o: $(SMOOTHINCLUDEDIR)/resources.h $(RESOURCEDIR)/version.rc $(RESOURCEDIR)/resources.rc
	$(ECHO) -n Compiling $(RESOURCEDIR)/resources.rc...
	$(RESCOMP) $(RESCOMP_OPTS) $(RESOURCEDIR)/resources.rc -o $(OBJECTDIR)/resources.o
	$(ECHO) done.

$(OBJECTDIR)/designer.o: $(DESIGNERDIR)/designer.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(DESIGNERDIR)/designer.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(DESIGNERDIR)/designer.cpp -o $(OBJECTDIR)/designer.o
	$(ECHO) done.

$(OBJECTDIR)/edit_component.o: $(DESIGNERDIR)/edit_component.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(DESIGNERDIR)/edit_component.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(DESIGNERDIR)/edit_component.cpp -o $(OBJECTDIR)/edit_component.o
	$(ECHO) done.

$(OBJECTDIR)/tools.o: $(DESIGNERDIR)/tools.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(DESIGNERDIR)/tools.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(DESIGNERDIR)/tools.cpp -o $(OBJECTDIR)/tools.o
	$(ECHO) done.

$(OBJTESTDIR)/objtest.o: $(OBJTESTDIR)/objtest.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(OBJTESTDIR)/objtest.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(OBJTESTDIR)/objtest.cpp -o $(OBJTESTDIR)/objtest.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_instream.o: $(IOLIBDIR)/instream.cpp $(INCLUDEDIR)/instream-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/instream.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/instream.cpp -o $(OBJECTDIR)/iolib_instream.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_iolib.o: $(IOLIBDIR)/iolib.cpp $(INCLUDEDIR)/iolib-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/iolib.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/iolib.cpp -o $(OBJECTDIR)/iolib_iolib.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_iolibfilter.o: $(IOLIBDIR)/iolibfilter.cpp $(INCLUDEDIR)/iolibfilter-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/iolibfilter.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/iolibfilter.cpp -o $(OBJECTDIR)/iolib_iolibfilter.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_iolibstream.o: $(IOLIBDIR)/iolibstream.cpp $(INCLUDEDIR)/iolibstream-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/iolibstream.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/iolibstream.cpp -o $(OBJECTDIR)/iolib_iolibstream.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_outstream.o: $(IOLIBDIR)/outstream.cpp $(INCLUDEDIR)/outstream-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/outstream.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/outstream.cpp -o $(OBJECTDIR)/iolib_outstream.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_socks4.o: $(IOLIBDIR)/socks4.cpp $(INCLUDEDIR)/iolib-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/socks4.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/socks4.cpp -o $(OBJECTDIR)/iolib_socks4.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_socks5.o: $(IOLIBDIR)/socks5.cpp $(INCLUDEDIR)/iolib-cxx.h
	$(ECHO) -n Compiling $(IOLIBDIR)/socks5.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBDIR)/socks5.cpp -o $(OBJECTDIR)/iolib_socks5.o
	$(ECHO) done.

$(OBJECTDIR)/picture_picture.o: $(PICTUREDIR)/picture.cpp $(INCLUDEDIR)/picture.h
	$(ECHO) -n Compiling $(PICTUREDIR)/picture.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(PICTUREDIR)/picture.cpp -o $(OBJECTDIR)/picture_picture.o
	$(ECHO) done.

$(OBJECTDIR)/libpci_codecs.o: $(LIBPCIDIR)/codecs.cpp $(INCLUDEDIR)/codecs.h
	$(ECHO) -n Compiling $(LIBPCIDIR)/codecs.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LIBPCIDIR)/codecs.cpp -o $(OBJECTDIR)/libpci_codecs.o
	$(ECHO) done.

$(OBJECTDIR)/libpci_pciio.o: $(LIBPCIDIR)/pciio.cpp $(INCLUDEDIR)/pciio.h
	$(ECHO) -n Compiling $(LIBPCIDIR)/pciio.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LIBPCIDIR)/pciio.cpp -o $(OBJECTDIR)/libpci_pciio.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_blocksort.o: $(LIBBZ2DIR)/blocksort.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/blocksort.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/blocksort.c -o $(OBJECTDIR)/libbz2_blocksort.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_bzlib.o: $(LIBBZ2DIR)/bzlib.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/bzlib.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/bzlib.c -o $(OBJECTDIR)/libbz2_bzlib.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_compress.o: $(LIBBZ2DIR)/compress.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/compress.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/compress.c -o $(OBJECTDIR)/libbz2_compress.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_crctable.o: $(LIBBZ2DIR)/crctable.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/crctable.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/crctable.c -o $(OBJECTDIR)/libbz2_crctable.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_decompress.o: $(LIBBZ2DIR)/decompress.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/decompress.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/decompress.c -o $(OBJECTDIR)/libbz2_decompress.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_huffman.o: $(LIBBZ2DIR)/huffman.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/huffman.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/huffman.c -o $(OBJECTDIR)/libbz2_huffman.o
	$(ECHO) done.

$(OBJECTDIR)/libbz2_randtable.o: $(LIBBZ2DIR)/randtable.c
	$(ECHO) -n Compiling $(LIBBZ2DIR)/randtable.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBBZ2DIR)/randtable.c -o $(OBJECTDIR)/libbz2_randtable.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_iolib-filters.o: $(INCLUDEDIR)/iolib-filters-cxx.h $(IOLIBFILTERSDIR)/iolib-filters.cpp
	$(ECHO) -n Compiling $(IOLIBFILTERSDIR)/iolib-filters.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBFILTERSDIR)/iolib-filters.cpp -o $(OBJECTDIR)/iolib_iolib-filters.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_filter-bz2.o: $(INCLUDEDIR)/filter-bz2-cxx.h $(IOLIBFILTERSDIR)/filter-bz2.cpp
	$(ECHO) -n Compiling $(IOLIBFILTERSDIR)/filter-bz2.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBFILTERSDIR)/filter-bz2.cpp -o $(OBJECTDIR)/iolib_filter-bz2.o
	$(ECHO) done.

$(OBJECTDIR)/iolib_filter-xor.o: $(INCLUDEDIR)/filter-xor-cxx.h $(IOLIBFILTERSDIR)/filter-xor.cpp
	$(ECHO) -n Compiling $(IOLIBFILTERSDIR)/filter-xor.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(IOLIBFILTERSDIR)/filter-xor.cpp -o $(OBJECTDIR)/iolib_filter-xor.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_catalog.o: $(LIBXMLDIR)/catalog.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/catalog.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/catalog.c -o $(OBJECTDIR)/libxml_catalog.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_encoding.o: $(LIBXMLDIR)/encoding.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/encoding.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/encoding.c -o $(OBJECTDIR)/libxml_encoding.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_entities.o: $(LIBXMLDIR)/entities.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/entities.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/entities.c -o $(OBJECTDIR)/libxml_entities.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_error.o: $(LIBXMLDIR)/error.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/error.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/error.c -o $(OBJECTDIR)/libxml_error.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_globals.o: $(LIBXMLDIR)/globals.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/globals.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/globals.c -o $(OBJECTDIR)/libxml_globals.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_hash.o: $(LIBXMLDIR)/hash.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/hash.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/hash.c -o $(OBJECTDIR)/libxml_hash.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_list.o: $(LIBXMLDIR)/list.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/list.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/list.c -o $(OBJECTDIR)/libxml_list.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_parser.o: $(LIBXMLDIR)/parser.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/parser.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/parser.c -o $(OBJECTDIR)/libxml_parser.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_parserInternals.o: $(LIBXMLDIR)/parserInternals.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/parserInternals.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/parserInternals.c -o $(OBJECTDIR)/libxml_parserInternals.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_SAX.o: $(LIBXMLDIR)/SAX.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/SAX.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/SAX.c -o $(OBJECTDIR)/libxml_SAX.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_threads.o: $(LIBXMLDIR)/threads.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/threads.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/threads.c -o $(OBJECTDIR)/libxml_threads.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_tree.o: $(LIBXMLDIR)/tree.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/tree.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/tree.c -o $(OBJECTDIR)/libxml_tree.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_uri.o: $(LIBXMLDIR)/uri.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/uri.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/uri.c -o $(OBJECTDIR)/libxml_uri.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_valid.o: $(LIBXMLDIR)/valid.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/valid.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/valid.c -o $(OBJECTDIR)/libxml_valid.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_xlink.o: $(LIBXMLDIR)/xlink.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/xlink.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/xlink.c -o $(OBJECTDIR)/libxml_xlink.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_xmlIO.o: $(LIBXMLDIR)/xmlIO.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/xmlIO.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/xmlIO.c -o $(OBJECTDIR)/libxml_xmlIO.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_xmlmemory.o: $(LIBXMLDIR)/xmlmemory.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/xmlmemory.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/xmlmemory.c -o $(OBJECTDIR)/libxml_xmlmemory.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_xpath.o: $(LIBXMLDIR)/xpath.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/xpath.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/xpath.c -o $(OBJECTDIR)/libxml_xpath.o
	$(ECHO) done.

$(OBJECTDIR)/libxml_xpointer.o: $(LIBXMLDIR)/xpointer.c
	$(ECHO) -n Compiling $(LIBXMLDIR)/xpointer.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBXMLDIR)/xpointer.c -o $(OBJECTDIR)/libxml_xpointer.o
	$(ECHO) done.

$(OBJECTDIR)/libiconv_iconv.o: $(LIBICONVDIR)/iconv.c
	$(ECHO) -n Compiling $(LIBICONVDIR)/iconv.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBICONVDIR)/iconv.c -o $(OBJECTDIR)/libiconv_iconv.o
	$(ECHO) done.

$(OBJECTDIR)/libiconv_localcharset.o: $(LIBICONVDIR)/localcharset.c
	$(ECHO) -n Compiling $(LIBICONVDIR)/localcharset.c...
	$(COMPILER) $(COMPILER_OPTS) $(LIBICONVDIR)/localcharset.c -o $(OBJECTDIR)/libiconv_localcharset.o
	$(ECHO) done.

$(OBJECTDIR)/lisa_lisa.o: $(LISADIR)/lisa_win32.cpp
	$(ECHO) -n Compiling $(LISADIR)/lisa_win32.cpp...
	$(COMPILER) $(COMPILER_OPTS) $(LISADIR)/lisa_win32.cpp -o $(OBJECTDIR)/lisa_lisa.o
	$(ECHO) done.

$(SAMPLE1DIR)/beat.o: $(SAMPLE1DIR)/beat.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(SAMPLE1DIR)/beat.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(SAMPLE1DIR)/beat.cpp -o $(SAMPLE1DIR)/beat.o
	$(ECHO) done.

$(SAMPLE2DIR)/test.o: $(SAMPLE2DIR)/test.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(SAMPLE2DIR)/test.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(SAMPLE2DIR)/test.cpp -o $(SAMPLE2DIR)/test.o
	$(ECHO) done.

$(SAMPLE3DIR)/testkey.o: $(SAMPLE3DIR)/testkey.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(SAMPLE3DIR)/testkey.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(SAMPLE3DIR)/testkey.cpp -o $(SAMPLE3DIR)/testkey.o
	$(ECHO) done.

$(SAMPLE4DIR)/plugintest.o: $(SAMPLE4DIR)/plugintest.cpp $(DLLNAME) $(SAMPLE4DLL)
	$(ECHO) -n Compiling $(SAMPLE4DIR)/plugintest.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(SAMPLE4DIR)/plugintest.cpp -o $(SAMPLE4DIR)/plugintest.o
	$(ECHO) done.

$(SAMPLE4DIR)/plugin.o: $(SAMPLE4DIR)/plugin.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(SAMPLE4DIR)/plugin.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) -D__SMOOTH_PLUGIN_DLL__ $(SAMPLE4DIR)/plugin.cpp -o $(SAMPLE4DIR)/plugin.o
	$(ECHO) done.

$(SAMPLE5DIR)/mditest.o: $(SAMPLE5DIR)/mditest.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(SAMPLE5DIR)/mditest.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(SAMPLE5DIR)/mditest.cpp -o $(SAMPLE5DIR)/mditest.o
	$(ECHO) done.

$(TUTORIAL01DIR)/t01.o: $(TUTORIAL01DIR)/t01.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(TUTORIAL01DIR)/t01.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(TUTORIAL01DIR)/t01.cpp -o $(TUTORIAL01DIR)/t01.o
	$(ECHO) done.

$(TUTORIAL02DIR)/t02.o: $(TUTORIAL02DIR)/t02.cpp $(DLLNAME)
	$(ECHO) -n Compiling $(TUTORIAL02DIR)/t02.cpp...
	$(COMPILER) $(EXECOMPILER_OPTS) $(TUTORIAL02DIR)/t02.cpp -o $(TUTORIAL02DIR)/t02.o
	$(ECHO) done.
