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
#include "string.h"

#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE shInstance, HINSTANCE shPrevInstance, LPSTR sszCmdLine, int siCmdShow)
{
	SMOOTH	*smoothclass = new SMOOTH();

	hInstance		= shInstance;
	hPrevInstance		= shPrevInstance;
	szCmdLine		= SMOOTHString(sszCmdLine);
	iCmdShow		= siCmdShow;

	SMOOTH::Init();

	if (SMOOTH::GetVersionString() != (SMOOTHString) SMOOTH_VERSION)
	{
		if (IDNO == SMOOTH::MessageBox("This program might not be compatible with the SMOOTH library\ninstalled on your system. Continue execution?", "Warning", MB_YESNO, IDI_QUESTION))
		{
			SMOOTH::Free();

			return 0;
		}
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

	smoothclass->Main();

	SMOOTH::Free();

	delete smoothclass;

	return 0;
}
#else
int main()
{
	SMOOTH	*smoothclass = new SMOOTH();

	SMOOTH::Init();

	if (SMOOTH::GetVersionString() != (SMOOTHString) SMOOTH_VERSION)
	{
		SMOOTH::Free();

		return 0;

	}

	smoothclass->Main();

	SMOOTH::Free();

	delete smoothclass;

	return 0;
}
#endif

#endif
