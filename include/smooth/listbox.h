 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTBOX_
#define _H_OBJSMOOTH_LISTBOX_

namespace smooth
{
	namespace GUI
	{
		class ListBox;
		class ListBoxHeader;
		class Scrollbar;
	};
};

#include "widget.h"
#include "list.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 LF_NORMAL		= 0;
		const Int	 LF_HIDESCROLLBAR	= 1;
		const Int	 LF_ALLOWRESELECT	= 2;
		const Int	 LF_MULTICHECKBOX	= 4;
		const Int	 LF_HIDEHEADER		= 8;

		class SMOOTHAPI ListBox : public Widget, public List
		{
			private:
				Int		 entryCount;

				Scrollbar	*scrollbar;
				Int		 scrollbarPos;
				Int		 lastScrollbarPos;

				ListBoxHeader	*header;

				Void		 ScrollbarProc();
				Void		 DrawEntryText(String, Rect, Int);
			public:
						 ListBox(Point, Size);
						~ListBox();

				ListEntry	*AddEntry(String, Int = -1);
				Int		 ModifyEntry(Int, String);
				Int		 RemoveEntry(Int);
				Int		 SelectEntry(Int);
				Int		 RemoveAll();

				Int		 AddTab(String, Int = 0);
				Int		 GetNthTabOffset(Int);
				Int		 GetNthTabWidth(Int);

				Int		 ClearTabs();

				virtual Int	 Show();
				virtual Int	 Hide();

				virtual Int	 Activate();
				virtual Int	 Deactivate();

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 ScrollUp(Int = 1);
				Int		 ScrollDown(Int = 1);
		};
	};

	SMOOTHVAR Int OBJ_LISTBOX;
};

#endif
