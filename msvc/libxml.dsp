# Microsoft Developer Studio Project File - Name="libxml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libxml - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "libxml.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "libxml.mak" CFG="libxml - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "libxml - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "libxml - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libxml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "libxml___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../include" /D "WIN32" /D "__WIN32__" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__SMOOTH_DLL__" /D "LIBXML_STATIC" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libxml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libxml___Win32_Debug"
# PROP BASE Intermediate_Dir "libxml___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "libxml___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__SMOOTH_DLL__" /D "LIBXML_STATIC" /YX /FD /GZ /c
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

# Name "libxml - Win32 Release"
# Name "libxml - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\libraries\libxml\chvalid.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\dict.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\encoding.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\entities.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\error.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\globals.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\hash.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\list.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\parser.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\parserInternals.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\SAX2.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\threads.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\tree.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\uri.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\valid.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlIO.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlmemory.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlreader.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlsave.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlstring.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libxml\xmlwriter.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "libxml"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=..\include\libxml\chvalid.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\debugXML.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\dict.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\encoding.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\entities.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\globals.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\hash.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\HTMLparser.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\HTMLtree.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\list.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\nanoftp.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\nanohttp.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\parser.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\parserInternals.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\SAX.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\SAX2.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\threads.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\tree.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\uri.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\valid.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xlink.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlautomata.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlerror.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlexports.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlIO.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlmemory.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlreader.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlregexp.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlsave.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlstring.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlversion.h
# End Source File
# Begin Source File

SOURCE=..\include\libxml\xmlwriter.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\include\libxml.h
# End Source File
# Begin Source File

SOURCE=..\include\win32config.h
# End Source File
# End Group
# End Target
# End Project
