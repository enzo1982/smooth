# Microsoft Developer Studio Project File - Name="IOLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=IOLib - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "IOLib.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "IOLib.mak" CFG="IOLib - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "IOLib - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "IOLib - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IOLib - Win32 Release"

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

!ELSEIF  "$(CFG)" == "IOLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IOLib___Win32_Debug"
# PROP BASE Intermediate_Dir "IOLib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "IOLib___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__SMOOTH_DLL__" /YX /FD /GZ /c
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

# Name "IOLib - Win32 Release"
# Name "IOLib - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "drivers"

# PROP Default_Filter ""
# Begin Group "ansi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\ansi\driver_ansi.cpp
# End Source File
# End Group
# Begin Group "memory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\memory\driver_memory.cpp
# End Source File
# End Group
# Begin Group "posix"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\posix\driver_posix.cpp
# End Source File
# End Group
# Begin Group "socket"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\socket\driver_socket.cpp
# End Source File
# End Group
# Begin Group "zero"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\zero\driver_zero.cpp
# End Source File
# End Group
# Begin Group "socks4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\socks4\driver_socks4.cpp
# End Source File
# End Group
# Begin Group "socks5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\socks5\driver_socks5.cpp
# End Source File
# End Group
# Begin Group "unicode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\drivers\unicode\driver_unicode.cpp
# End Source File
# End Group
# End Group
# Begin Group "filters"

# PROP Default_Filter ""
# Begin Group "xor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\filters\xor\filter_xor.cpp
# End Source File
# End Group
# Begin Group "bzip2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\libraries\iolib\filters\bzip2\filter_bzip2.cpp
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=..\libraries\iolib\instream.cpp
# End Source File
# Begin Source File

SOURCE=..\libraries\iolib\iolib.cpp
# End Source File
# Begin Source File

SOURCE=..\libraries\iolib\iolibdriver.cpp
# End Source File
# Begin Source File

SOURCE=..\libraries\iolib\iolibfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\libraries\iolib\iolibstream.cpp
# End Source File
# Begin Source File

SOURCE=..\libraries\iolib\outstream.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "iolib"

# PROP Default_Filter ""
# Begin Group "drivers-headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_ansi.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_memory.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_posix.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_socket.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_socks4.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_socks5.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_unicode.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\drivers\driver_zero.h
# End Source File
# End Group
# Begin Group "filters-headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\iolib\filters\filter_bzip2.h
# End Source File
# Begin Source File

SOURCE=..\include\iolib\filters\filter_xor.h
# End Source File
# End Group
# Begin Source File

SOURCE="..\include\iolib\instream-cxx.h"
# End Source File
# Begin Source File

SOURCE="..\include\iolib\iolibdriver-cxx.h"
# End Source File
# Begin Source File

SOURCE="..\include\iolib\iolibfilter-cxx.h"
# End Source File
# Begin Source File

SOURCE="..\include\iolib\iolibstream-cxx.h"
# End Source File
# Begin Source File

SOURCE="..\include\iolib\outstream-cxx.h"
# End Source File
# End Group
# Begin Source File

SOURCE="..\include\iolib-cxx.h"
# End Source File
# End Group
# End Target
# End Project
