 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LISTBOXHEADER_
#define _H_OBJSMOOTH_LISTBOXHEADER_

namespace smooth
{
	namespace GUI
	{
		class ListBoxHeader;
		class ListBox;
	};
};

#include "../../widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ListBoxHeader : public Widget
		{
			private:
				ListBox			*listBox;

				Int			 moveTab;
				Bool			 innerLoop;

				Array<String>		 tabNames;
				Array<Int>		 tabWidths;
				Array<Bool>		 tabChecked;
			public:
				static const Int	 classID;

							 ListBoxHeader(ListBox *);
							~ListBoxHeader();

				Int			 AddTab(String, Int);

				Int			 ClearTabs();

				virtual Int		 Paint(Int);
				Int			 Process(Int, Int, Int);

				Int			 GetNOfTabs();
				Int			 GetNthTabOffset(Int);
				Int			 GetNthTabWidth(Int);

				Int			 UpdateMetrics();
		};
	};
};

#endif
