 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_CONTAINER
#define H_OBJSMOOTH_CONTAINER

namespace smooth
{
	namespace GUI
	{
		class Container;
	};
};

#include "entry.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI Container : public Widget
		{
			protected:
				Array<Bool>		 createdEntry;
				Array<Entry *, Void *>	 elementOrder;
			public:
				static const Short	 classID;

							 Container();
				virtual			~Container();

				virtual Int		 EnableLocking(Bool = True);

				virtual Int		 Add(Widget *);
				virtual Int		 Remove(Widget *);

				virtual Int		 RemoveAllEntries();

				virtual Int		 MoveEntry(Int, Int);
			accessors:
				virtual Int		 Length() const			{ return elementOrder.Length(); }

				virtual Entry		*GetNthEntry(Int n) const	{ return elementOrder.GetNth(n); }
			signals:
				Signal0<Void>		 onChangeEntryOrder;
		};
	};
};

#endif
