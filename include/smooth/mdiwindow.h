 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

#include "window.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI MDIWindow : public Window
		{
			public:
				 MDIWindow(String title = NIL);
				~MDIWindow();
		};
	};

	SMOOTHVAR Int OBJ_MDIWINDOW;
};

#endif
