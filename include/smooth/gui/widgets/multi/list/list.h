 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_LIST_
#define _H_OBJSMOOTH_LIST_

namespace smooth
{
	namespace GUI
	{
		class List;
	};
};

#include "listentry.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI List : public Widget
		{
			public:
				static const Int		 classID;

								 List();
				virtual				~List();

				ListEntry			*AddEntry(const String &);
				Int				 RemoveEntry(ListEntry *);

				Int				 Clear();

				Int				 GetNOfEntries();
				ListEntry			*GetNthEntry(Int);

				Int				 SelectEntry(ListEntry *);
				ListEntry			*GetSelectedEntry();

				Int				 SelectNthEntry(Int);
				Int				 GetSelectedEntryNumber();

				Int				 SelectEntry(const String &);
			signals:
				Signal1<Void, ListEntry *>	 onSelectEntry;
				Signal1<Void, ListEntry *>	 onMarkEntry;
		};
	};
};

#endif
