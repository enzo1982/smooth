 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINAPPLICATION_
#define _H_OBJSMOOTH_WINAPPLICATION_

#include "loop.h"
#include "version.h"
#include "stk.h"
#include "messagebox.h"

extern "C"
{
	BOOL WINAPI DllMain(HINSTANCE shInstance, DWORD reason, LPVOID reserved)
	{
		SMOOTH	*smoothclass = new SMOOTH();

		switch (reason)
		{
			case DLL_PROCESS_ATTACH:
				hInstance	= shInstance;
				hPrevInstance	= NULL;
				iCmdShow	= SW_SHOW;

				SMOOTH::Init();

				if (SMOOTH::GetVersionString() != (SMOOTHString) SMOOTH_VERSION)
				{
					SMOOTH::MessageBox("This DLL might not be compatible with the\nSMOOTH library installed on your system.", "Warning", MB_OK, IDI_QUESTION);
				}

				if (SMOOTH::Setup::enableUnicode)
				{
					wchar_t	*buffer = new wchar_t [MAX_PATH];

					GetCurrentDirectoryW(MAX_PATH, buffer);

					SMOOTH::SetStartDirectory(buffer);

					delete [] buffer;
				}
				else
				{
					char	*buffer = new char [MAX_PATH];

					GetCurrentDirectoryA(MAX_PATH, buffer);

					SMOOTH::SetStartDirectory(buffer);

					delete [] buffer;
				}

				smoothclass->AttachDLL();

				break;
			case DLL_PROCESS_DETACH:
				smoothclass->DetachDLL();

				SMOOTH::Free();

				break;
			case DLL_THREAD_ATTACH:
				break;
			case DLL_THREAD_DETACH:
				break;
		}

		delete smoothclass;

		return true;
	}
}

#endif
