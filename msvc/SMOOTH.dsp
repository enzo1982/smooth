# Microsoft Developer Studio Project File - Name="SMOOTH" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SMOOTH - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "SMOOTH.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "SMOOTH.mak" CFG="SMOOTH - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "SMOOTH - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SMOOTH - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SMOOTH_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SMOOTH_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "SMOOTH___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SMOOTH_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__SMOOTH_DLL__" /D "UNICODE" /D "_UNICODE" /U "_DLL" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 IOLib.lib IOLibFilters.lib libbz2.lib lisa.lib libpci.lib libpicture.lib libxml.lib iconv.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib msvcrt.lib oldnames.lib /nologo /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /libpath:"Build"
# SUBTRACT LINK32 /verbose /pdb:none /incremental:no /map

!ENDIF 

# Begin Target

# Name "SMOOTH - Win32 Release"
# Name "SMOOTH - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "classes"

# PROP Default_Filter "*.cpp"
# Begin Group "basic"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\basic\application.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\background.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\container.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\containertype.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\object.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\objectmanager.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\objectproperties.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\objecttype.cpp
# End Source File
# End Group
# Begin Group "internal"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\internal\toolwindow.cpp
# End Source File
# End Group
# Begin Group "layer"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\layer\activearea.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\arrows.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\button.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\editbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\groupbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\hyperlink.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\layer.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\optionbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\progressbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\slider.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\tabregister.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\text.cpp
# End Source File
# End Group
# Begin Group "list"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\list\combobox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\list\list.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\list\listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\list\listentry.cpp
# End Source File
# End Group
# Begin Group "mdi"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\mdi\mdiclient.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\mdi\mdiwindow.cpp
# End Source File
# End Group
# Begin Group "menu"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\menu\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\menu\menubar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\menu\menuentry.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\menu\popupmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\menu\popupview.cpp
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\misc\date.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\string.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\surface.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\surfacegdi.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\time.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\timer.cpp
# End Source File
# End Group
# Begin Group "primitives"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\primitives\line.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\primitives\point.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\primitives\primitive.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\primitives\rect.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\primitives\size.cpp
# End Source File
# End Group
# Begin Group "threads"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\threads\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\threadmanager.cpp
# End Source File
# End Group
# Begin Group "tree"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\tree\tree.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\tree\treeentry.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\tree\treeview.cpp
# End Source File
# End Group
# Begin Group "window"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\window\client.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\divisionbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\drag.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\statusbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\titlebar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\tooltip.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\window.cpp
# End Source File
# End Group
# Begin Group "xml"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\classes\xml\attribute.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\xml\document.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\xml\node.cpp
# End Source File
# End Group
# End Group
# Begin Group "i18n"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\i18n\afrikaans.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\basque.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\breton.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\bulgarian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\catalan.cpp
# End Source File
# Begin Source File

SOURCE="..\i18n\chinese-simplified.cpp"
# End Source File
# Begin Source File

SOURCE=..\i18n\chinese.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\croatian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\czech.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\danish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\dutch.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\english.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\esperanto.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\estonian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\finnish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\french.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\galician.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\german.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\greek.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\hebrew.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\hungarian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\i18n.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\icelandic.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\irish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\italian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\japanese.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\korean.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\latin.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\latvian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\lithuanian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\macedonian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\maori.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\norwegian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\occitan.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\polish.cpp
# End Source File
# Begin Source File

SOURCE="..\i18n\portuguese-brazil.cpp"
# End Source File
# Begin Source File

SOURCE=..\i18n\portuguese.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\romanian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\russian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\serbian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\slovak.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\slovenian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\spanish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\swedish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\tamil.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\thai.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\turkish.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\ukrainian.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\walloon.cpp
# End Source File
# Begin Source File

SOURCE=..\i18n\welsh.cpp
# End Source File
# End Group
# Begin Group "dialogs"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\dialogs\colordlg.cpp
# End Source File
# Begin Source File

SOURCE=..\dialogs\dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\dialogs\dirdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\dialogs\filedlg.cpp
# End Source File
# End Group
# Begin Group "miscsources"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\misc\binary.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\color.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\definitions.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\loop.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\mathtools.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\messagebox.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\metrics.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\splashscreen.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\stk.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\toolkit.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "smooth"

# PROP Default_Filter "*.h"
# Begin Group "i18nheaders"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\i18n\afrikaans.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\basque.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\breton.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\bulgarian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\catalan.h
# End Source File
# Begin Source File

SOURCE="..\include\smooth\i18n\chinese-simplified.h"
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\chinese.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\croatian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\czech.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\danish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\dutch.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\english.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\esperanto.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\estonian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\finnish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\french.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\galician.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\german.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\greek.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\hebrew.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\hungarian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\icelandic.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\irish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\italian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\japanese.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\korean.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\latin.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\latvian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\lithuanian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\macedonian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\maori.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\norwegian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\occitan.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\polish.h
# End Source File
# Begin Source File

SOURCE="..\include\smooth\i18n\portuguese-brazil.h"
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\portuguese.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\romanian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\russian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\serbian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\slovak.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\slovenian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\spanish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\swedish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\tamil.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\thai.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\turkish.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\ukrainian.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\walloon.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n\welsh.h
# End Source File
# End Group
# Begin Group "templates"

# PROP Default_Filter "*.h"
# Begin Group "array"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\templates\array\array.cpp

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\array.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\entry.cpp

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\entry.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\value.cpp

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\value.h
# End Source File
# End Group
# End Group
# Begin Group "types"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\types\bool.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\byte.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\float.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\int.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\short.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\void.h
# End Source File
# End Group
# Begin Group "xmlheaders"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\xml\attribute.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\document.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\node.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\activearea.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\application.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\array.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\arrows.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\background.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\binary.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\button.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\checkbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\client.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\color.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\colordlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\combobox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\container.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\containertype.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\date.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\definitions.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialog.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dirdlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\divisionbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dllmain.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\drag.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\editbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\filedlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\groupbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\hyperlink.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\i18n.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\layer.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\line.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\list.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\listbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\loop.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mathtools.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mdiclient.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mdiwindow.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menubar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\messagebox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\metrics.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mutex.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\object.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\objectmanager.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\objectproperties.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\objecttype.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\optionbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\point.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\popupmenu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\popupview.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\primitive.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\progressbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\rect.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\resources.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\scrollbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\size.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\slider.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\splashscreen.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\statusbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\stk.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\stream.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\string.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\surface.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\surfacegdi.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\tabregister.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\text.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\thread.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threadmanager.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\time.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\timer.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\titlebar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\toolkit.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\tooltip.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\toolwindow.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\tree.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\treeview.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\version.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\winapp.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\windll.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth.h
# End Source File
# Begin Source File

SOURCE=..\include\smoothdll.h
# End Source File
# Begin Source File

SOURCE=..\include\smoothplugin.h
# End Source File
# Begin Source File

SOURCE=..\include\smoothx.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\resources\resources\binary\icon.ico
# End Source File
# Begin Source File

SOURCE=..\resources\icon.rc

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x407 /i "\devel\projects\mpsd\src\smooth\resources" /i "I:\devel\projects\mpsd\src\smooth\resources"
# ADD RSC /l 0x407 /i "\devel\projects\mpsd\src\smooth\resources" /i "I:\devel\projects\mpsd\src\smooth" /i "I:\devel\projects\mpsd\src\smooth\resources" /d "__WIN32__" /d "__SMOOTH_DLL__"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\resources\iconbmp.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\resources\linkhand.rc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\resources\resources.rc

!IF  "$(CFG)" == "SMOOTH - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SMOOTH - Win32 Debug"

# ADD BASE RSC /l 0x407 /i "\devel\projects\mpsd\src\smooth\resources" /i "I:\devel\projects\mpsd\src\smooth\resources"
# ADD RSC /l 0x407 /i "\devel\projects\mpsd\src\smooth\resources" /i "I:\devel\projects\mpsd\src\smooth" /i "I:\devel\projects\mpsd\src\smooth\resources" /d "__WIN32__" /d "__SMOOTH_DLL__"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\resources\version.rc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
