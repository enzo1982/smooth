 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

#include "object.h"
#include "array.h"
#include "listentry.h"

namespace smooth
{
	class SMOOTHAPI List : public Array<ListEntry *>
	{
		protected:
			Bool			 entrysizesset;

			virtual ListEntry	*AddEntry(Int, String);
			virtual Int		 ModifyEntry(Int, String);
			virtual Int		 RemoveEntry(Int);

			Void			 GetSize();
			Void			 GetListEntriesSize();
		public:
						 List();
						~List();

			virtual Void		 Cleanup();

			virtual Int		 SelectEntry(Int);
			ListEntry		*GetSelectedEntry();
	};
};

#endif
