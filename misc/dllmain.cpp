 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/dllmain.h>
#include <smooth/toolkit.h>

#ifdef __WIN32__
#include <windows.h>

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
#endif
