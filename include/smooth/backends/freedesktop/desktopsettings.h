 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_DESKTOPSETTINGS
#define H_OBJSMOOTH_DESKTOPSETTINGS

namespace smooth
{
	namespace Backends
	{
		class DesktopSettings;
	};
};

#include "../../definitions.h"

namespace smooth
{
	namespace Backends
	{
		/* Desktop environment settings shared by
		 * the XLib and Wayland backends.
		 */
		class DesktopSettings
		{
			private:
				static String		 FindExecutable(const String &);

				static Bool		 UpdateColorsGnome();
				static Bool		 UpdateColorsKDE();
			public:
				static Void		 BlockTimerInterrupts(Bool);

				static Void		 InitDependencies();

				static Void		 UpdateColors();

				static String		 QueryGSettings(const String &, const String &);
				static String		 QueryKDESettings(const String &, const String &);
				static String		 QueryXfConf(const String &, const String &);
		};
	};
};

#endif
