 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
#include "i18n.h"
#include "graphics/bitmap.h"

#if defined MessageBox
#undef MessageBox
#endif

#if defined LoadImage
#undef LoadImage
#endif

namespace smooth
{
	class SMOOTHAPI SMOOTH
	{
		public:
#ifdef __WIN32__
			static I18n::Translator	*i18n;

			static HBITMAP		 LoadImage(String, Int, String);

			static Int		 MessageBox(String, String, Int, char *);
			static Int		 MessageBox(String, String, Int, wchar_t *);
			static Int		 SplashScreen(const GUI::Bitmap &, Int);
#endif
	};
};

#endif
