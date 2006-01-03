# Microsoft Developer Studio Project File - Name="fribidi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fribidi - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "fribidi.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "fribidi.mak" CFG="fribidi - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "fribidi - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "fribidi - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fribidi - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "fribidi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "fribidi___Win32_Debug"
# PROP BASE Intermediate_Dir "fribidi___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "fribidi___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../libraries/fribidi2" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "fribidi - Win32 Release"
# Name "fribidi - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-bidi-types.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-bidi.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-env.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-joining-types.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-joining.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-mem.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-mirroring.c"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-run.c"
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\fribidi.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\libraries\fribidi2\bidi-types.h"
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\common.h
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\debug.h
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\env.h
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-begindecls.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-bidi-types-list.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-bidi-types.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-bidi.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-common.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-config.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-enddecls.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-env.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-joining-types-list.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-joining-types.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-joining.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-mirroring.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-types.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-unicode-version.h"
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\fribidi-unicode.h"
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\fribidi.h
# End Source File
# Begin Source File

SOURCE="..\libraries\fribidi2\joining-types.h"
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\mem.h
# End Source File
# Begin Source File

SOURCE=..\libraries\fribidi2\run.h
# End Source File
# End Group
# End Target
# End Project
