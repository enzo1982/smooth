 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DIALOG_
#define _H_OBJSMOOTH_DIALOG_

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			class Dialog;
		};

		class Window;
	};
};

#include "../../definitions.h"
#include "../application/application.h"

namespace smooth
{
	namespace GUI
	{
		namespace Dialogs
		{
			class SMOOTHAPI Dialog : public Application
			{
				protected:
					String		 caption;
					Window		*parentWindow;
				public:
							 Dialog();
							~Dialog();

					virtual Int	 ShowDialog() = 0;

					Int		 SetCaption(String);
					Int		 SetParentWindow(Window *);
			};
		};
	};
};

#endif
