 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/string.h>
#include <smooth/loop.h>
#include <smooth/graphics/window.h>
#include <smooth/stk.h>
#include <smooth/array.h>
#include <smooth/shlobjmini.h>

#include <iolib-cxx.h>

#ifdef __WIN32__
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace smooth;

Array<HDC>	 contexts;
Array<int>	 contextCounts;

LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderA)(PBROWSEINFOA);
LPITEMIDLIST	 (WINAPI *ex_SHBrowseForFolderW)(PBROWSEINFOW);
BOOL		 (WINAPI *ex_SHGetPathFromIDListA)(LPCITEMIDLIST, LPSTR);
BOOL		 (WINAPI *ex_SHGetPathFromIDListW)(LPCITEMIDLIST, LPWSTR);

HMODULE	 shelldll;

void SHLObjMini_Init()
{
	shelldll = LoadLibraryA("shell32.dll");

	ex_SHBrowseForFolderA = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOA)) GetProcAddress(shelldll, "SHBrowseForFolderA");
	ex_SHBrowseForFolderW = (LPITEMIDLIST (WINAPI *)(PBROWSEINFOW)) GetProcAddress(shelldll, "SHBrowseForFolderW");
	ex_SHGetPathFromIDListA = (BOOL (WINAPI *)(LPCITEMIDLIST, LPSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListA");
	ex_SHGetPathFromIDListW = (BOOL (WINAPI *)(LPCITEMIDLIST, LPWSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListW");
}

void SHLObjMini_Deinit()
{
	FreeLibrary(shelldll);
}

HDC GetContext(GUI::Window *wnd)
{
	int	 code = 0;

	if (wnd != NIL) code = (int) wnd->hwnd;

	if (contextCounts.GetEntry(code) == 0)
	{
		contexts.AddEntry(GetWindowDC((HWND) code), code);
		contextCounts.AddEntry(1, code);
	}
	else
	{
		contextCounts.SetEntry(code, contextCounts.GetEntry(code) + 1);
	}

	return contexts.GetEntry(code);
}

void FreeContext(GUI::Window *wnd, HDC hdc)
{
	int	 code = 0;

	if (wnd != NIL) code = (int) wnd->hwnd;

	contextCounts.SetEntry(code, contextCounts.GetEntry(code) - 1);

	if (contextCounts.GetEntry(code) == 0)
	{
		ReleaseDC((HWND) code, contexts.GetEntry(code));

		contexts.RemoveEntry(code);
		contextCounts.RemoveEntry(code);
	}
}
