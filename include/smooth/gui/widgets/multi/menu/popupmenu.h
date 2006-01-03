 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPMENU_
#define _H_OBJSMOOTH_POPUPMENU_

namespace smooth
{
	namespace GUI
	{
		class PopupMenu;
		class ToolWindow;
	};
};

#include "menu.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI PopupMenu : public Menu
		{
			private:
				PopupMenu		*prevPopup;
				PopupMenu		*nextPopup;

				ToolWindow		*toolWindow;
			public:
				static const Int	 classID;

							 PopupMenu();
				virtual			~PopupMenu();

				virtual Int		 Show();
				virtual Int		 Hide();

				MenuEntry		*AddEntry(const String & = NIL, const Bitmap & = NIL, Menu * = NIL, Bool * = NIL, Int * = NIL, Int iCode = 0);
			signals:
				static Signal0<Void>	 internalClosePopups;
			slots:
				Void			 InternalClosePopups();

				Void			 OnToolWindowPaint();
		};
	};
};

#endif
