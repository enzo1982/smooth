 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MENUBAR_
#define _H_OBJSMOOTH_MENUBAR_

namespace smooth
{
	namespace GUI
	{
		class Menubar;
		class Surface;
	};
};

#include "../widget.h"
#include "menu.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 MB_GRAYSCALE	= 0;
		const Int	 MB_COLOR	= 1;

		class SMOOTHAPI Menubar : public Menu
		{
			private:
				Int			 bitmapSize;
			slots:
				Void			 OnRegister();
			public:
				static const Int	 classID;

							 Menubar();
							~Menubar();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 SetBitmapSize(Int);

				Surface			*GetDrawSurface();
		};
	};
};

#endif
