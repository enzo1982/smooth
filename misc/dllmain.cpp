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

LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderA)(PBROWSEINFOA);
LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderW)(PBROWSEINFOW);
BOOL		 (WINAPI *ex_SHGetPathFromIDListA)(LPCITEMIDLIST, LPSTR);
BOOL		 (WINAPI *ex_SHGetPathFromIDListW)(LPCITEMIDLIST, LPWSTR);

HMODULE		 shellDLL;

void SHLObjMini_Init()
{
	shellDLL = LoadLibraryA("shell32.dll");

	ex_SHBrowseForFolderA = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOA)) GetProcAddress(shellDLL, "SHBrowseForFolderA");
	ex_SHBrowseForFolderW = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOW)) GetProcAddress(shellDLL, "SHBrowseForFolderW");
	ex_SHGetPathFromIDListA = (BOOL (WINAPI *)(LPCITEMIDLIST, LPSTR)) GetProcAddress(shellDLL, "SHGetPathFromIDListA");
	ex_SHGetPathFromIDListW = (BOOL (WINAPI *)(LPCITEMIDLIST, LPWSTR)) GetProcAddress(shellDLL, "SHGetPathFromIDListW");
}

void SHLObjMini_Deinit()
{
	FreeLibrary(shellDLL);
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
