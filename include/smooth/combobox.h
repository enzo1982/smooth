 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_COMBOBOX_
#define _H_OBJSMOOTH_COMBOBOX_

namespace smooth
{
	namespace GUI
	{
		class ComboBox;
		class Layer;
		class ToolWindow;
		class ListBox;
	};
};

#include "widget.h"
#include "list.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 CB_NORMAL	= 0;
		const Int	 CB_HOTSPOTONLY	= 1;

		class SMOOTHAPI ComboBox : public Widget, public List
		{
			private:
				Int		 entryCount;

				ToolWindow	*toolWindow;
				Layer		*layer;
				ListBox		*listBox;

				Bool		 listBoxOpen;
				Bool		 closeListBox;

				Void		 ListBoxProc();
			public:
						 ComboBox(Point, Size);
						~ComboBox();

				ListEntry	*AddEntry(String, Int = -1);
				Int		 ModifyEntry(Int, String);
				Int		 RemoveEntry(Int);
				Int		 SelectEntry(Int);
				Int		 RemoveAll();

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);
		};
	};

	SMOOTHVAR Int OBJ_COMBOBOX;
};

#endif
