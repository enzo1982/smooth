 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPMENU_
#define _H_OBJSMOOTH_POPUPMENU_

#include "definitions.h"

namespace smooth
{
	namespace GUI
	{
		class PopupMenu;
		class Window;
		class ToolWindow;
		class PopupView;
	};
};

#include "widget.h"
#include "menu.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 POPUP_NORMAL	= 0;
		const Int	 POPUP_FINISHED	= 1;
		const Int	 POPUP_PENDING	= 2;

		class SMOOTHAPI PopupMenu : public Widget, public Menu
		{
			friend class Window;
			friend class PopupView;
			private:
				ToolWindow	*toolwnd;
				PopupView	*popupView;

				PopupMenu	*prevPopup;
				PopupMenu	*nextPopup;
			public:
				static Int	 status;

						 PopupMenu();
				 		~PopupMenu();

				Void		 MenuToPopup(Menu *);

				Int		 Process(Int, Int, Int);

				Int		 Show();
				Int		 Hide();
		};
	};

	SMOOTHVAR Int OBJ_POPUP;
};

#endif
