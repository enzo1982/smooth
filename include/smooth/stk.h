 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_STK_
#define _H_OBJSMOOTH_STK_

namespace smooth
{
	class SMOOTH;
	class Object;

	namespace GUI
	{
		class Widget;
		class Window;
	};
};

#include "definitions.h"
#include "string.h"
#include "rect.h"

#if defined MessageBox
#undef MessageBox
#endif

namespace smooth
{
	class SMOOTHAPI SMOOTH
	{
		public:
			static Void		 SetLanguage(Int);

#ifdef __WIN32__
			static HBITMAP		 LoadImage(String, Int, String);

			static Void		 SendMessage(GUI::Window *, Int, Int, Int);

			static Int		 MessageBox(String, String, Int, char *);
			static Int		 MessageBox(String, String, Int, wchar_t *);
			static Int		 SplashScreen(HBITMAP, Int);
#endif
	};

#ifdef __SMOOTH_DLL__
#ifdef __WIN32__
	HBITMAP	 GrayscaleBitmap(HBITMAP);
	HBITMAP	 DetectTransparentRegions(HBITMAP);
#endif

	Bool	 Affected(GUI::Widget *, Rect &);
#endif
};

#endif
