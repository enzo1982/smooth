 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINAPPLICATION_
#define _H_OBJSMOOTH_WINAPPLICATION_

#include "loop.h"
#include "version.h"
#include "system.h"
#include "messagebox.h"
#include "application.h"
#include "messagebox.h"

using namespace smooth;

extern "C"
{
	BOOL WINAPI DllMain(HINSTANCE shInstance, DWORD reason, LPVOID reserved)
	{
		switch (reason)
		{
			case DLL_PROCESS_ATTACH:
				hInstance	= shInstance;
				hPrevInstance	= NULL;
				iCmdShow	= SW_SHOW;

				Init();

				if (System::GetVersionString() != (String) SMOOTH_VERSION)
				{
					QuickMessage("This DLL might not be compatible with the\nSMOOTH library installed on your system.", "Warning", MB_OK, IDI_QUESTION);
				}

				Application::GetStartupDirectory();
				Application::GetApplicationDirectory();

				AttachDLL();

				break;
			case DLL_PROCESS_DETACH:
				DetachDLL();

				Free();

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
