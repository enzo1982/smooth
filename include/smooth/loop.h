 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LOOP_
#define _H_OBJSMOOTH_LOOP_

#include "definitions.h"
#include "misc/string.h"

namespace smooth
{
#ifdef __WIN32__
	SMOOTHVAR HINSTANCE	 hInstance;
	SMOOTHVAR HINSTANCE	 hPrevInstance;
	SMOOTHVAR String	 szCmdLine;
	SMOOTHVAR int		 iCmdShow;

	SMOOTHVAR HICON		 SMOOTHICON;
#endif

	Void SMOOTHAPI	 Init();
	Void SMOOTHAPI	 Free();
	Int SMOOTHAPI	 Loop();

#ifdef __SMOOTH_DLL__
	extern bool	 initializing;
	extern bool	 loopActive;
	extern int	 peekLoop;

	Void	 GetColors();
	Void	 GetDefaultFont();
#endif
};

#endif
