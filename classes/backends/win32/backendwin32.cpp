 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/window/backends/gdi/windowgdi.h>

#include <iconv.h>

size_t	 (*iconv)(iconv_t, char **, size_t *, char **, size_t *)	= NIL;
iconv_t	 (*iconv_open)(const char *, const char *)			= NIL;
int	 (*iconv_close)(iconv_t)					= NIL;

HINSTANCE  iconvDLL	= NIL;
HINSTANCE  hDllInstance	= NIL;

S::Bool S::LoadIconvDLL()
{
	if (Setup::enableUnicode) iconvDLL = LoadLibraryW(GUI::Application::GetApplicationDirectory().Append("iconv"));
	else			  iconvDLL = LoadLibraryA(GUI::Application::GetApplicationDirectory().Append("iconv"));

	iconv		= (size_t (*)(iconv_t, char **, size_t *, char **, size_t *)) GetProcAddress(iconvDLL, "iconv");
	iconv_open	= (iconv_t (*)(const char *, const char *)) GetProcAddress(iconvDLL, "iconv_open");
	iconv_close	= (int (*)(iconv_t)) GetProcAddress(iconvDLL, "iconv_close");

	if (iconv	== NIL ||
	    iconv_open	== NIL ||
	    iconv_close == NIL)	{ FreeIconvDLL(); return False; }

	return True;
}

S::Void S::FreeIconvDLL()
{
	FreeLibrary(iconvDLL);

	iconv		= NIL;
	iconv_open	= NIL;
	iconv_close	= NIL;
}

#ifndef SMOOTH_STATIC

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

S::Backends::Backend *CreateBackendWin32()
{
	return new S::Backends::BackendWin32();
}

S::Int	 backendWin32Tmp = S::Backends::Backend::AddBackend(&CreateBackendWin32);

S::Backends::BackendWin32::BackendWin32()
{
	type = BACKEND_WIN32;
}

S::Backends::BackendWin32::~BackendWin32()
{
}

S::Int S::Backends::BackendWin32::Init()
{
	WORD	 wVersionRequested = MAKEWORD(2,2);
	WSADATA	 wsaData;

	WSAStartup(wVersionRequested, &wsaData);

	HDC	 dc = GetWindowDC(0);

	Setup::FontSize = (Float) GetDeviceCaps(dc, LOGPIXELSY) / 96;

	ReleaseDC(0, dc);

	GUI::WindowGDI::InitMouseNotifier();

	return Success();
}

S::Int S::Backends::BackendWin32::Deinit()
{
	GUI::WindowGDI::FreeMouseNotifier();

	WSACleanup();

	return Success();
}
