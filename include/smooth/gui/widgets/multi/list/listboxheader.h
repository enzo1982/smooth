 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
				Int			 moveTab;
				Bool			 innerLoop;

				Array<String>		 tabNames;
				Array<Int>		 tabWidths;
				Array<Int>		 tabOrientations;
				Array<Bool>		 tabChecked;
			public:
				static const Int	 classID;

							 ListBoxHeader(const Point &, const Size &);
				virtual			~ListBoxHeader();

				virtual Int		 Paint(Int);
				virtual Int		 Process(Int, Int, Int);

				Int			 AddTab(const String &, Int, Int);

				Int			 GetNOfTabs();
				Int			 GetNthTabOffset(Int);
				Int			 GetNthTabWidth(Int);
				Int			 GetNthTabOrientation(Int);

				Int			 RemoveAllTabs();

				Int			 UpdateMetrics();
		};
	};
};

#endif
