 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_MDIWINDOW_
#define _H_OBJSMOOTH_MDIWINDOW_

namespace smooth
{
	namespace GUI
	{
		class MDIWindow;
	};
};

#include "window/window.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MDIWindow : public Window
		{
			public:
				static const Int	 classID;

							 MDIWindow(String = NIL);
							~MDIWindow();
		};
	};
};

#endif
