 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/win32/backendwin32.h>

#include <winsock2.h>

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

	return Success;
}

S::Int S::Backends::BackendWin32::Deinit()
{
	WSACleanup();

	return Success;
}
