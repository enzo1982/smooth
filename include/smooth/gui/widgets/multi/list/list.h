 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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
	class List;
};

#include "../../../../object.h"
#include "../../../../array.h"
#include "listentry.h"

namespace smooth
{
	class SMOOTHAPI List : public Array<ListEntry *>
	{
		protected:
			List			*referenceList;
			Bool			 addNil;

			Bool			 IsListSane();
			Int			 SynchronizeList();

			virtual Void		 CheckFlags();
		public:
						 List();
						~List();

			virtual ListEntry	*AddEntry(String, Int);
			virtual Int		 ModifyEntry(Int, String);
			virtual Int		 RemoveEntry(Int);
			virtual Int		 RemoveAll();

			virtual Int		 SelectEntry(Int);
			ListEntry		*GetSelectedEntry();

			Int			 SetReferenceList(List *);
	};
};

#endif
