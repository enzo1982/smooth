########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../../..

# Enter object files here:
OBJECTS	  = colordlg.o dialog.o fontdlg.o messagebox.o splashscreen.o tipodaydlg.o

# Enter addition commands for targets all and clean here:
ALLCMD1   = $(call makein,directory)
ALLCMD2   = $(call makein,file)
ALLCMD3   = $(call makein,icons)
CLEANCMD1 = $(call cleanin,directory)
CLEANCMD2 = $(call cleanin,file)
CLEANCMD3 = $(call cleanin,icons)

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
