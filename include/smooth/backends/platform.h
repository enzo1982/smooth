 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_PLATFORM
#define H_OBJSMOOTH_PLATFORM

namespace smooth
{
	namespace Backends
	{
		class Platform;
	};
};

#include "../definitions.h"

namespace smooth
{
	namespace Backends
	{
		/* Windowing platforms that can be selected at runtime.
		 */
		const Short	 PLATFORM_NONE	  = 0;
		const Short	 PLATFORM_X11	  = 1;
		const Short	 PLATFORM_WAYLAND = 2;

		/* Selects one of several windowing platforms compiled into smooth
		 * at runtime and activates the backend components belonging to it.
		 *
		 * The platform can be forced by passing --display:x11 or
		 * --display:wayland on the command line.
		 */
		class Platform
		{
			private:
				static Short		 selected;
				static char		 requested[32];

				static Int		 GetProbeIndex(Short);
			public:
				/* Request a platform by name. Must be called before
				 * smooth is initialized.
				 */
				static Void		 SetRequested(const char *);

				/* Register a function installing a backend component
				 * (window, surface, event, ...) for a platform.
				 */
				static Int		 AddComponent(Short, Int (*)());

				/* Register an availability check for a platform. Platforms
				 * with higher priority are preferred if available.
				 */
				static Int		 AddProbe(Short, Int, Bool (*)());

				/* Select a platform and install its components.
				 */
				static Short		 Select();

				static Short		 GetSelected()		{ return selected; }
				static Bool		 IsSelected(Short platform) { return selected == platform; }

				static const char	*GetPlatformName(Short);
		};
	};
};

#endif
