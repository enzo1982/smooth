 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TOOLWINDOW_
#define _H_OBJSMOOTH_TOOLWINDOW_

namespace smooth
{
	namespace GUI
	{
		class ToolWindow;
	};

	class Object;
};

#include "definitions.h"
#include "window.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ToolWindow : public Window
		{
			private:
				Widget	*owner;
			public:
					 ToolWindow();
					~ToolWindow();

				Int	 SetOwner(Widget *);
				Int	 FreeOwner();

				Int	 Paint(Int);
				Int	 Process(Int, Int, Int);

				Int	 RegisterObject(Object *);

				Bool	 IsTypeCompatible(Int);
		};
	};

	SMOOTHVAR Int OBJ_TOOLWINDOW;
};

#endif
