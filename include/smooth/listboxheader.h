 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

#include "widget.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI ListBoxHeader : public Widget
		{
			private:
				ListBox		*listBox;

				Array<String>	 tabNames;
				Array<Int>	 tabWidths;
				Array<Bool>	 tabChecked;
			public:
						 ListBoxHeader(ListBox *);
						~ListBoxHeader();

				Int		 AddTab(String, Int);

				virtual Int	 Paint(Int);
				Int		 Process(Int, Int, Int);

				Int		 UpdateMetrics();
		};
	};

	SMOOTHVAR Int OBJ_LISTBOXHEADER;
};

#endif
