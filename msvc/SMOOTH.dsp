# Microsoft Developer Studio Project File - Name="smooth" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=smooth - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "smooth.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "smooth.mak" CFG="smooth - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "smooth - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "smooth - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "smooth - Win32 Release"

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

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "smooth___Win32_Debug"
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
# ADD LINK32 IOLib.lib libbz2.lib lisa.lib libxml.lib iconv.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib imm32.lib libc.lib oldnames.lib /nologo /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /libpath:"Build"
# SUBTRACT LINK32 /verbose /pdb:none /incremental:no /map

!ENDIF 

# Begin Target

# Name "smooth - Win32 Release"
# Name "smooth - Win32 Debug"
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

SOURCE=..\classes\basic\input.cpp
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
# Begin Source File

SOURCE=..\classes\basic\setup.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\widget.cpp
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

SOURCE=..\classes\layer\button.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\layer\droparea.cpp
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

SOURCE=..\classes\layer\image.cpp
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

SOURCE=..\classes\layer\tabwidget.cpp
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

SOURCE=..\classes\list\listboxheader.cpp
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

SOURCE=..\classes\misc\binary.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\config.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\datetime.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\font.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\i18n.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\math.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\string.cpp
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
# Begin Group "posixbackend"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\threads\posix\mutexposix.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\threads\posix\semaphoreposix.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\threads\posix\threadposix.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "win32backend"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\threads\win32\mutexwin32.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\win32\semaphorewin32.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\win32\threadwin32.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\threads\mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\mutexbackend.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\semaphore.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\semaphorebackend.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\thread.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\threads\threadbackend.cpp
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
# Begin Group "windowgdi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\window\gdi\windowgdi.cpp
# End Source File
# End Group
# Begin Group "windowxlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\window\xlib\windowxlib.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\window\client.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\divider.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\drag.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\window\shortcut.cpp
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
# Begin Source File

SOURCE=..\classes\window\windowbackend.cpp
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
# Begin Group "graphics"

# PROP Default_Filter "*.cpp"
# Begin Group "gdi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\graphics\gdi\bitmapgdi.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\gdi\surfacegdi.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\graphics\bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\bitmapbackend.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\surface.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\surfacebackend.cpp
# End Source File
# End Group
# Begin Group "dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\dialogs\colordlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\dirdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\filedlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\fontdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\messagebox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\splashscreen.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\dialogs\tipodaydlg.cpp
# End Source File
# End Group
# Begin Group "backends"

# PROP Default_Filter ""
# Begin Group "backendssdl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\backends\sdl\backendsdl.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "backendswin32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\backends\win32\backendwin32.cpp
# End Source File
# End Group
# Begin Group "backendsxlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\backends\xlib\backendxlib.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\backends\backend.cpp
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Group "sdl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\system\sdl\timersdl.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "systemwin32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\system\win32\eventwin32.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\system\win32\timerwin32.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\system\event.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\system\eventbackend.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\system\system.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\system\timer.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\system\timerbackend.cpp
# End Source File
# End Group
# End Group
# Begin Group "miscsources"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=..\misc\codecs.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\color.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\dllmain.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\loop.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\metrics.cpp
# End Source File
# Begin Source File

SOURCE=..\misc\pciio.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "smooth"

# PROP Default_Filter "*.h"
# Begin Group "templates"

# PROP Default_Filter "*.h"
# Begin Group "array"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\templates\array\array.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\array.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\entry.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\entry.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\value.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\array\value.h
# End Source File
# End Group
# Begin Group "signals_templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\templates\signals\instancen.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\signals\methodt.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\signals\signaln.h
# End Source File
# End Group
# Begin Group "callbacks_templates"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\templates\callbacks\callbackn.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\callbacks\instancen.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\callbacks\methodt.h
# End Source File
# End Group
# End Group
# Begin Group "types"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\types\bool.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\float.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\int.h
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
# Begin Group "signals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\signals\instance.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\signals\method.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\signals\signal.h
# End Source File
# End Group
# Begin Group "callbacks"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\callbacks\callback.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\callbacks\instance.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\callbacks\method.h
# End Source File
# End Group
# Begin Group "i18n"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\smooth\i18n\smooth_de.h
# End Source File
# End Group
# Begin Group "graphicsheaders"

# PROP Default_Filter ""
# Begin Group "gdiheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\graphics\gdi\bitmapgdi.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\gdi\surfacegdi.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\graphics\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\bitmapbackend.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\surface.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\surfacebackend.h
# End Source File
# End Group
# Begin Group "dialogsheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\dialogs\colordlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\dialog.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\dirdlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\filedlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\fontdlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\messagebox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\splashscreen.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dialogs\tipodaydlg.h
# End Source File
# End Group
# Begin Group "menuheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\menu\menu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menu\menubar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menu\menuentry.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menu\popupmenu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\menu\popupview.h
# End Source File
# End Group
# Begin Group "threadsheaders"

# PROP Default_Filter ""
# Begin Group "posix"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\threads\posix\mutexposix.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\posix\semaphoreposix.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\posix\threadposix.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\threads\win32\mutexwin32.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\win32\semaphorewin32.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\win32\threadwin32.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\threads\mutex.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\mutexbackend.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\semaphore.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\semaphorebackend.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\thread.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\threadbackend.h
# End Source File
# End Group
# Begin Group "backendsheaders"

# PROP Default_Filter ""
# Begin Group "backendssdlheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\backends\sdl\backendsdl.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "backendswin32headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\backends\win32\backendwin32.h
# End Source File
# End Group
# Begin Group "backendsxlibheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\backends\xlib\backendxlib.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\backends\backend.h
# End Source File
# End Group
# Begin Group "systemheaders"

# PROP Default_Filter ""
# Begin Group "sdlheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\system\sdl\timersdl.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "systemwin32headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\system\win32\eventwin32.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system\win32\timerwin32.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\system\event.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system\eventbackend.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system\system.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system\timer.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\system\timerbackend.h
# End Source File
# End Group
# Begin Group "mischeaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\misc\binary.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\config.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\datetime.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\font.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\i18n.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\math.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\string.h
# End Source File
# End Group
# Begin Group "windowheaders"

# PROP Default_Filter ""
# Begin Group "windowgdiheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\window\gdi\windowgdi.h
# End Source File
# End Group
# Begin Group "windowxlibheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\window\xlib\windowxlib.h

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\window\client.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\divider.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\drag.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\shortcut.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\statusbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\titlebar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\tooltip.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\window.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\window\windowbackend.h
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

SOURCE=..\include\smooth\button.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\callbacks.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\checkbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\codecs.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\color.h
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

SOURCE=..\include\smooth\definitions.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dllmain.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\droparea.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\editbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\groupbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\hyperlink.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\image.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\input.h
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

SOURCE=..\include\smooth\listboxheader.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\listentry.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\loop.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mdiclient.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\mdiwindow.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\metrics.h
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

SOURCE=..\include\smooth\pciio.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\point.h
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

SOURCE=..\include\smooth\setup.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\shlobjmini.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\signals.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\slider.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\tabwidget.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\text.h
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

SOURCE=..\include\smooth\widget.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\winapp.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\windll.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\resources\resources.rc

!IF  "$(CFG)" == "smooth - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# ADD BASE RSC /l 0x407 /i "\devel\projects\private\smooth\resources" /i "\devel\projects\mpsd\smooth\resources" /i "\devel\projects\mpsd\src\smooth\resources" /i "I:\devel\projects\mpsd\src\smooth\resources"
# ADD RSC /l 0x407 /i "\devel\projects\private\smooth\resources" /i "\devel\projects\mpsd\smooth\resources" /i "\devel\projects\mpsd\src\smooth\resources" /i "..\resources" /i ".." /d "__WIN32__" /d "__SMOOTH_DLL__"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
