 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOWBACKEND_
#define _H_OBJSMOOTH_WINDOWBACKEND_

namespace smooth
{
	namespace GUI
	{
		class WindowBackend;
	};
};

#include "../definitions.h"
#include "../misc/string.h"
#include "../primitives/point.h"
#include "../primitives/size.h"
#include "../callbacks.h"
#include "../graphics/surface.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 WINDOW_NONE = 0;

		class WindowBackend
		{
			protected:
				Int				 type;

				Surface				*nullSurface;
				Surface				*drawSurface;
			public:
								 WindowBackend(Void * = NIL);
				virtual				~WindowBackend();

				Int				 GetWindowType();

				virtual Void			*GetSystemWindow();

				virtual Surface			*GetDrawSurface();

				virtual Int			 Open(String, Point, Size, Int);
				virtual Int			 Close();

				virtual Int			 SetTitle(String);
				virtual Int			 SetIcon(Bitmap &);

				virtual Int			 SetMinimumSize(Size);
				virtual Int			 SetMaximumSize(Size);

				virtual Int			 Show();
				virtual Int			 Hide();

				virtual Rect			 GetRestoredWindowRect();

				virtual Int			 SetMetrics(const Point &, const Size &);

				virtual Int			 Minimize();

				virtual Int			 Maximize();
				virtual Int			 Restore();
			signals:
				Callback3<Int, Int, Int, Int>	 onEvent;
		};
	};
};

#endif
