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
# ADD LINK32 fribidi.lib IOLib.lib libbz2.lib lisa.lib libxml.lib iconv.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib imm32.lib libc.lib oldnames.lib /nologo /dll /debug /machine:I386 /nodefaultlib /pdbtype:sept /libpath:"Build"
# SUBTRACT LINK32 /verbose /pdb:none /incremental:no /map

!ENDIF 

# Begin Target

# Name "smooth - Win32 Release"
# Name "smooth - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "misc"

# PROP Default_Filter ""
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

SOURCE=..\misc\pciio.cpp
# End Source File
# End Group
# Begin Group "classes"

# PROP Default_Filter "*.cpp"
# Begin Group "basic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\basic\input.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\object.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\objecttype.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\basic\setup.cpp
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
# Begin Group "graphics"

# PROP Default_Filter ""
# Begin Group "graphicsxlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\graphics\xlib\bitmapxlib.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\graphics\xlib\surfacexlib.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "graphicsgdi"

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

SOURCE=..\classes\graphics\font.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\form.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\line.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\point.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\rect.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\size.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\surface.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\graphics\surfacebackend.cpp
# End Source File
# End Group
# Begin Group "files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\files\directory.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\files\file.cpp
# End Source File
# End Group
# Begin Group "xml"

# PROP Default_Filter ""
# Begin Group "xul"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\xml\xul\button.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\description.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\label.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\menubar.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\popupmenu.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\renderer.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\widget.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\xml\xul\window.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_XUL"

!ENDIF 

# End Source File
# End Group
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
# Begin Group "threads"

# PROP Default_Filter ""
# Begin Group "threadsposix"

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
# Begin Group "threadssdl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\threads\sdl\mutexsdl.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\threads\sdl\semaphoresdl.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\threads\sdl\threadsdl.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "threadswin32"

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
# Begin Group "system"

# PROP Default_Filter ""
# Begin Group "systemsdl"

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
# Begin Group "miscclasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\misc\args.cpp
# End Source File
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

SOURCE=..\classes\misc\i18n.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\math.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\misc\string.cpp
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Group "dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\dialogs\colordlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\dirdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\filedlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\fontdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\messagebox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\splashscreen.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\dialogs\tipodaydlg.cpp
# End Source File
# End Group
# Begin Group "widgets"

# PROP Default_Filter ""
# Begin Group "basicwidgets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\activearea.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\arrows.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\button.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\checkbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\client.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\divider.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\editbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\groupbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\hyperlink.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\image.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\optionbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\progressbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\scrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\slider.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\statusbar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\tabwidget.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\text.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\basic\titlebar.cpp
# End Source File
# End Group
# Begin Group "multi"

# PROP Default_Filter ""
# Begin Group "list"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\list\combobox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\list\list.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\list\listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\list\listboxheader.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\list\listentry.cpp
# End Source File
# End Group
# Begin Group "menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\menu\menu.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\menu\menubar.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\menu\menuentry.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\menu\popupmenu.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\menu\popupview.cpp
# End Source File
# End Group
# Begin Group "tree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\tree\tree.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\tree\treeentry.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\multi\tree\treeview.cpp
# End Source File
# End Group
# End Group
# Begin Group "special"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\widgets\special\dragcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\special\droparea.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\special\shortcut.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\special\tooltip.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\gui\widgets\container.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\containertype.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\layer.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\widgets\widget.cpp
# End Source File
# End Group
# Begin Group "window"

# PROP Default_Filter ""
# Begin Group "windowgdi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\window\gdi\windowgdi.cpp
# End Source File
# End Group
# Begin Group "windowxlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\window\xlib\windowxlib.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\classes\gui\window\toolwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\window\window.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\window\windowbackend.cpp
# End Source File
# End Group
# Begin Group "mdi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\mdi\client.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_MDI"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\classes\gui\mdi\window.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Intermediate_Dir "smooth___Win32_Debug_MDI"

!ENDIF 

# End Source File
# End Group
# Begin Group "application"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\classes\gui\application\application.cpp
# End Source File
# Begin Source File

SOURCE=..\classes\gui\application\background.cpp
# End Source File
# End Group
# End Group
# End Group
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "smooth"

# PROP Default_Filter "*.h"
# Begin Group "backendsheaders"

# PROP Default_Filter ""
# Begin Group "backendssdlheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\backends\sdl\backendsdl.h
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
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\backends\backend.h
# End Source File
# End Group
# Begin Group "templates"

# PROP Default_Filter ""
# Begin Group "array"

# PROP Default_Filter ""
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
# End Group
# Begin Group "extended"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\templates\extended\extended.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\extended\extended.h
# End Source File
# End Group
# Begin Group "signals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\templates\signals\callback.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\signals\signal.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\signals\signal_base.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\signals\signalr.h
# End Source File
# End Group
# Begin Group "slots"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slot_base.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slot_class.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slot_global.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slot_signal.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slotr_base.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slotr_class.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slotr_global.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slotr_signal.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\slots\slotr_value.h
# End Source File
# End Group
# Begin Group "buffer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\templates\buffer\buffer.cpp

!IF  "$(CFG)" == "smooth - Win32 Release"

!ELSEIF  "$(CFG)" == "smooth - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\include\smooth\templates\buffer\buffer.h
# End Source File
# End Group
# End Group
# Begin Group "filesheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\files\directory.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\files\file.h
# End Source File
# End Group
# Begin Group "graphicsheaders"

# PROP Default_Filter ""
# Begin Group "graphicsgdiheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\graphics\gdi\bitmapgdi.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\gdi\surfacegdi.h
# End Source File
# End Group
# Begin Group "graphicsxlibheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\graphics\xlib\bitmapxlib.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\xlib\surfacexlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\graphics\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\bitmapbackend.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\font.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\form.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\line.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\point.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\rect.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\size.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\surface.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\graphics\surfacebackend.h
# End Source File
# End Group
# Begin Group "i18n"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\i18n\smooth_de.h
# End Source File
# End Group
# Begin Group "mischeaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\misc\args.h
# End Source File
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

SOURCE=..\include\smooth\misc\i18n.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\math.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\misc\string.h
# End Source File
# End Group
# Begin Group "systemheaders"

# PROP Default_Filter ""
# Begin Group "systemsdlheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\system\sdl\timersdl.h
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
# Begin Group "xmlheaders"

# PROP Default_Filter ""
# Begin Group "xulheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\xml\xul\button.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\description.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\label.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\menubar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\popupmenu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\renderer.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\widget.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\xml\xul\window.h
# End Source File
# End Group
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
# Begin Group "types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\types\bool.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\float.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\generic.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\int.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\types\void.h
# End Source File
# End Group
# Begin Group "threadsheaders"

# PROP Default_Filter ""
# Begin Group "threadsposixheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\threads\posix\mutexposix.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\posix\semaphoreposix.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\posix\threadposix.h
# End Source File
# End Group
# Begin Group "threadssdlheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\threads\sdl\mutexsdl.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\sdl\semaphoresdl.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\threads\sdl\threadsdl.h
# End Source File
# End Group
# Begin Group "threadswin32headers"

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
# Begin Group "guiheaders"

# PROP Default_Filter ""
# Begin Group "dialogsheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\colordlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\dialog.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\dirdlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\filedlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\fontdlg.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\messagebox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\splashscreen.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\dialogs\tipodaydlg.h
# End Source File
# End Group
# Begin Group "applicationheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\application\application.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\application\background.h
# End Source File
# End Group
# Begin Group "mdiheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\mdi\client.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\mdi\window.h
# End Source File
# End Group
# Begin Group "widgetsheaders"

# PROP Default_Filter ""
# Begin Group "widgetsbasicheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\activearea.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\arrows.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\button.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\checkbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\client.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\divider.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\editbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\groupbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\hyperlink.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\image.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\optionbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\progressbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\scrollbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\slider.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\statusbar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\tabwidget.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\text.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\basic\titlebar.h
# End Source File
# End Group
# Begin Group "widgetsmultiheaders"

# PROP Default_Filter ""
# Begin Group "listheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\list\combobox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\list\list.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\list\listbox.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\list\listboxheader.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\list\listentry.h
# End Source File
# End Group
# Begin Group "menuheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\menu\menu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\menu\menubar.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\menu\menuentry.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\menu\popupmenu.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\menu\popupview.h
# End Source File
# End Group
# Begin Group "treeheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\tree\tree.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\multi\tree\treeview.h
# End Source File
# End Group
# End Group
# Begin Group "widgetsspecialheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\special\dragcontrol.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\special\droparea.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\special\shortcut.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\special\tooltip.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\container.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\containertype.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\layer.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\widgets\widget.h
# End Source File
# End Group
# Begin Group "windowheaders"

# PROP Default_Filter ""
# Begin Group "windowgdiheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\window\gdi\windowgdi.h
# End Source File
# End Group
# Begin Group "windowxlibheaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\smooth\gui\window\xlib\windowxlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\smooth\gui\window\toolwindow.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\window\window.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\gui\window\windowbackend.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\include\smooth\args.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\array.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\callbacks.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\codecs.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\color.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\definitions.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dll.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\dllmain.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\extended.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\input.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\loop.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\main.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\object.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\objecttype.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\pciio.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\resources.h
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

SOURCE=..\include\smooth\signalsr.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\slots.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\slotsr.h
# End Source File
# Begin Source File

SOURCE=..\include\smooth\version.h
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

# ADD BASE RSC /l 0x407 /i "\devel\projects\private\smooth\resources"
# ADD RSC /l 0x407 /i "\devel\projects\private\smooth\resources" /i "..\resources" /i ".." /d "__WIN32__" /d "__SMOOTH_DLL__"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
