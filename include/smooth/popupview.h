 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_POPUPVIEW_
#define _H_OBJSMOOTH_POPUPVIEW_

namespace smooth
{
	namespace GUI
	{
		class PopupView;
		class PopupMenu;
	};

	class Menu;
};

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI PopupView : public Widget
		{
			private:
				String		 backupStatusText;
			protected:
				PopupMenu	*myPopup;
				Menu		*realMenu;
			public:
						 PopupView(PopupMenu *, Menu *);
						~PopupView();

				Int		 Process(Int, Int, Int);
				Int		 Paint(Int);
		};
	};

	SMOOTHVAR Int OBJ_POPUPVIEW;
};

#endif
