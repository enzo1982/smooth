# Microsoft Developer Studio Project File - Name="libiconv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libiconv - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "libiconv.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "libiconv.mak" CFG="libiconv - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "libiconv - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "libiconv - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libiconv - Win32 Release"

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

!ELSEIF  "$(CFG)" == "libiconv - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Build"
# PROP Intermediate_Dir "libiconv___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Build\iconv.lib"

!ENDIF 

# Begin Target

# Name "libiconv - Win32 Release"
# Name "libiconv - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\libraries\libiconv\aliases.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\armscii_8.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ascii.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\big5.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\big5hkscs.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ces_big5.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ces_gbk.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cjk_variants.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_1.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_15.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_3.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_4.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_4a.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_4b.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_5.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_6.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_7.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cns11643_inv.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\config.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\converters.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1133.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1250.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1251.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1252.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1253.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1254.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1255.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1256.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1257.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp1258.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp850.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp862.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp866.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp874.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp932.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp932ext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp936ext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp949.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp950.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\cp950ext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\euc_cn.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\euc_jp.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\euc_kr.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\euc_tw.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\flags.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\flushwc.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gb18030.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gb18030ext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gb18030uni.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gb2312.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gbk.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gbkext1.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gbkext2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\gbkext_inv.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\georgian_academy.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\georgian_ps.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\hkscs.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\hp_roman8.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\hz.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iconv.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_cn.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_cnext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_jp.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_jp1.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_jp2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso2022_kr.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso646_cn.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso646_jp.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_1.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_10.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_13.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_14.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_15.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_16.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_3.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_4.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_5.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_6.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_7.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_8.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\iso8859_9.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\isoir165.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\isoir165ext.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\java.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\jisx0201.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\jisx0208.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\jisx0212.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\johab.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\johab_hangul.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\koi8_r.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\koi8_ru.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\koi8_u.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ksc5601.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\libcharset.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\libcharset_config.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\localcharset.c
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\loop_unicode.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\loop_wchar.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\loops.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_arabic.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_centraleurope.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_croatian.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_cyrillic.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_greek.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_hebrew.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_iceland.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_roman.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_romania.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_thai.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_turkish.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mac_ukraine.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\mulelao.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\nextstep.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\sjis.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\tcvn.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\tis620.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\translit.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs2be.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs2internal.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs2le.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs2swapped.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs4.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs4be.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs4internal.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs4le.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\ucs4swapped.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\uhc_1.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\uhc_2.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf16.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf16be.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf16le.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf32.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf32be.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf32le.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf7.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\utf8.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\vietcomb.h
# End Source File
# Begin Source File

SOURCE=..\libraries\libiconv\viscii.h
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\iconv.h
# End Source File
# End Group
# End Target
# End Project
