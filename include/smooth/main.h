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

#include "system.h"
#include "loop.h"
#include "version.h"
#include "string.h"
#include "application.h"
#include "dialogs/messagebox.h"

using namespace smooth;

#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE shInstance, HINSTANCE shPrevInstance, LPSTR sszCmdLine, int siCmdShow)
{
	hInstance		= shInstance;
	hPrevInstance		= shPrevInstance;
	szCmdLine		= String(sszCmdLine);
	iCmdShow		= siCmdShow;

	Init();

	if (System::GetVersionString() != (String) SMOOTH_VERSION)
	{
		if (IDNO == QuickMessage("This program might not be compatible with the SMOOTH library\ninstalled on your system. Continue execution?", "Warning", MB_YESNO, IDI_QUESTION))
		{
			Free();

			return -1;
		}
	}

	Application::GetStartupDirectory();
	Application::GetApplicationDirectory();

	Int	 retValue = Main();

	Free();

	return retValue;
}
#else
int main()
{
	Init();

	if (System::GetVersionString() != (String) SMOOTH_VERSION)
	{
		Free();

		return -1;

	}

	Int	 retValue = Main();

	Free();

	return retValue;
}
#endif

#endif
