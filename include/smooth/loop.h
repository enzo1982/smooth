 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LOOP_
#define _H_OBJSMOOTH_LOOP_

#include "definitions.h"
#include "string.h"

namespace smooth
{
#ifdef __WIN32__
	SMOOTHVAR HINSTANCE	 hInstance;
	SMOOTHVAR HINSTANCE	 hPrevInstance;
	SMOOTHVAR String	 szCmdLine;
	SMOOTHVAR int		 iCmdShow;

	SMOOTHVAR HICON		 SMOOTHICON;

	Void SMOOTHAPI	 Init();
	Void SMOOTHAPI	 Free();
	Int SMOOTHAPI	 Loop();
#endif

#ifdef __SMOOTH_DLL__

#ifdef __WIN32__
	extern HCURSOR		 ARROW;
	extern HCURSOR		 IBEAM;
	extern HCURSOR		 LINKHAND;
#endif

	extern HBITMAP		 DEFAULTICON;

	extern bool		 initializing;
	extern bool		 loopActive;
	extern int		 peekLoop;

	Void	 GetColors();
#endif
};

#endif
