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
	class ListBox;
	class Scrollbar;
};

#include "object.h"
#include "list.h"

namespace smooth
{
	class SMOOTHAPI ListBox : public Object, public List
	{
		private:
			Int		 entryCount;

			Scrollbar	*scrollbar;
			Bool		 needScrollbar;
			Int		 scrollbarPos;
			Int		 lastScrollbarPos;

			Bool		 allowReselect;

			Void		 ScrollbarProc();
		public:
					 ListBox(Point, Size);
					~ListBox();

			Entry		*AddEntry(String);
			Int		 ModifyEntry(Int, String);
			Int		 RemoveEntry(Int);
			Int		 SelectEntry(Int);
			Void		 Cleanup();

			virtual Int	 Show();
			virtual Int	 Hide();

			virtual Int	 Paint(Int);
			Int		 Process(Int, Int, Int);

			Int		 AllowReselect(Bool);
	};

	SMOOTHVAR Int OBJ_LISTBOX;
};

#endif
