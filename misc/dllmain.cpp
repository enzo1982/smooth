 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dllmain.h>
#include <smooth/shlobjmini.h>
#include <smooth/gui/application/application.h>
#include <iconv.h>

LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderA)(PBROWSEINFOA) = NIL;
LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderW)(PBROWSEINFOW) = NIL;
BOOL		 (WINAPI *ex_SHGetPathFromIDListA)(LPCITEMIDLIST, LPSTR) = NIL;
BOOL		 (WINAPI *ex_SHGetPathFromIDListW)(LPCITEMIDLIST, LPWSTR) = NIL;

HMODULE		 shellDLL;

void SHLObjMini_Init()
{
	shellDLL = LoadLibraryA("shell32.dll");

	ex_SHBrowseForFolderA	= (LPITEMIDLIST (WINAPI *)(PBROWSEINFOA)) GetProcAddress(shellDLL, "SHBrowseForFolderA");
	ex_SHBrowseForFolderW	= (LPITEMIDLIST (WINAPI *)(PBROWSEINFOW)) GetProcAddress(shellDLL, "SHBrowseForFolderW");
	ex_SHGetPathFromIDListA	= (BOOL (WINAPI *)(LPCITEMIDLIST, LPSTR)) GetProcAddress(shellDLL, "SHGetPathFromIDListA");
	ex_SHGetPathFromIDListW	= (BOOL (WINAPI *)(LPCITEMIDLIST, LPWSTR)) GetProcAddress(shellDLL, "SHGetPathFromIDListW");
}

void SHLObjMini_Deinit()
{
	FreeLibrary(shellDLL);

	ex_SHBrowseForFolderA	= NIL;
	ex_SHBrowseForFolderW	= NIL;
	ex_SHGetPathFromIDListA	= NIL;
	ex_SHGetPathFromIDListW	= NIL;
}

size_t	 (*iconv)(iconv_t, const char **, size_t *, char **, size_t *) = NIL;
iconv_t	 (*iconv_open)(const char *, const char *) = NIL;
int	 (*iconv_close)(iconv_t) = NIL;

HMODULE	 iconvDLL = NIL;

S::Bool S::LoadIconvDLL()
{
	iconvDLL = LoadLibraryA(GUI::Application::GetApplicationDirectory().Append("iconv.dll"));

	if (iconvDLL == NIL) return False;

	iconv		= (size_t (*)(iconv_t, const char **, size_t *, char **, size_t *)) GetProcAddress(iconvDLL, "iconv");
	iconv_open	= (iconv_t (*)(const char *, const char *)) GetProcAddress(iconvDLL, "iconv_open");
	iconv_close	= (int (*)(iconv_t)) GetProcAddress(iconvDLL, "iconv_close");

	if (iconv == NIL)	{ FreeIconvDLL(); return False; }
	if (iconv_open == NIL)	{ FreeIconvDLL(); return False; }
	if (iconv_close == NIL)	{ FreeIconvDLL(); return False; }

	return True;
}

S::Void S::FreeIconvDLL()
{
	FreeLibrary(iconvDLL);

	iconv		= NIL;
	iconv_open	= NIL;
	iconv_close	= NIL;
}

HINSTANCE	 hDllInstance = NIL;

#ifndef __SMOOTH_STATIC__

extern "C"
{
	BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
			case DLL_PROCESS_ATTACH:
				hDllInstance = hInst;

				break;
			case DLL_PROCESS_DETACH:
				break;
			case DLL_THREAD_ATTACH:
				break;
			case DLL_THREAD_DETACH:
				break;
		}

		return true;
	}
}

#endif
