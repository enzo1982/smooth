 /* The smooth Class Library
  * Copyright (C) 1998-2022 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_BACKENDXLIB
#define H_OBJSMOOTH_BACKENDXLIB

namespace smooth
{
	namespace Backends
	{
		class BackendXLib;
	};
};

#include "../backend.h"

#include <stdlib.h>

namespace X11
{
	extern "C"
	{
#		define True	1
#		define False	0
#		define Bool	int
#		define Success	0

#		include <X11/Xlib.h>

#		undef True
#		undef False
#		undef Bool
#		undef Success
	}
};

namespace smooth
{
	namespace Backends
	{
		const Short	 BACKEND_XLIB = 1;

		class BackendXLib : public Backend
		{
			private:
				static X11::Display	*display;
				static X11::XIM		 im;

				static String		 FindExecutable(const String &);

				static Bool		 UpdateColorsGnome();
				static Bool		 UpdateColorsKDE();
			public:
							 BackendXLib();
				virtual			~BackendXLib();

				Int			 Init();
				Int			 Deinit();
				
				static Void		 UpdateColors();

				static X11::Display	*GetDisplay();
				static X11::XIM		 GetIM();

				static String		 QueryGSettings(const String &, const String &);
				static String		 QueryKDESettings(const String &, const String &);
				static String		 QueryXfConf(const String &, const String &);
		};
	};
};

#endif
