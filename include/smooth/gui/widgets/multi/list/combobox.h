 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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

#include "list.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 CB_NORMAL	= 0;
		const Int	 CB_HOTSPOTONLY	= 1;

		class SMOOTHAPI ComboBox : public List
		{
			private:
				ToolWindow		*toolWindow;
				Layer			*layer;
				ListBox			*listBox;

				Bool			 closeListBox;
				ListEntry		*prevSelectedEntry;

				Void			 CloseListBox();
			public:
				static const Int	 classID;

							 ComboBox(Point, Size);
							~ComboBox();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);
			slots:
				Void			 ListBoxProc();
		};
	};
};

#endif
