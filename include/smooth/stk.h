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

extern size_t	 (*ex_iconv)(iconv_t, const char **, size_t *, char **, size_t *);
extern iconv_t	 (*ex_iconv_open)(const char *, const char *);
extern int	 (*ex_iconv_close)(iconv_t);

namespace smooth
{
	class SMOOTHAPI SMOOTH
	{
		public:
#ifdef __WIN32__
			static I18n::Translator	*i18n;

			static HBITMAP		 LoadImage(String, Int, String);

			static Void		 SendMessage(GUI::Window *, Int, Int, Int);

			static Int		 MessageBox(String, String, Int, char *);
			static Int		 MessageBox(String, String, Int, wchar_t *);
			static Int		 SplashScreen(const GUI::Bitmap &, Int);
#endif
	};

#ifdef __SMOOTH_DLL__
#ifdef __WIN32__
	Bool	 Affected(GUI::Widget *, Rect &);
#endif

	Bool	 LoadIconvDLL();
	Void	 FreeIconvDLL();
#endif
};

#endif
