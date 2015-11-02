########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALLIB   = libnsucd.a
MYCCOPTS    = -I$(SRCDIR) -I$(SRCDIR)/$(SMOOTH_PATH)/include/nsucd -I$(SRCDIR)/$(SMOOTH_PATH)/include/nsucd/support

# Enter object files here:
OBJECTS	    = CharDistribution.o JpCntx.o LangBulgarianModel.o LangCyrillicModel.o LangGreekModel.o LangHebrewModel.o LangHungarianModel.o LangThaiModel.o nsBig5Prober.o nsCharSetProber.o nsEscCharsetProber.o nsEscSM.o nsEUCJPProber.o nsEUCKRProber.o nsEUCTWProber.o nsGB2312Prober.o nsHebrewProber.o nsLatin1Prober.o nsMBCSGroupProber.o nsMBCSSM.o nsSBCharSetProber.o nsSBCSGroupProber.o nsSJISProber.o nsUniversalDetector.o nsUTF8Prober.o

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
