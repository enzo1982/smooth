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
#include "rect.h"

namespace smooth
{
	class SMOOTHAPI List
	{
		public: class SMOOTHAPI Entry
		{
			public:
				Int	 code;
				ProcMember;
				Void	*procParam;
				String	 text;

				Bool	 chk;
				Bool	 clk;
				Int	 size;
				Bool	 sizeset;
				Rect	 rect;

					 Entry(Int);
					~Entry();
		};

		protected:
			Bool		 entrysizesset;
			Int		 nOfEntries;

			Entry		*AddListEntry(Int, String, ProcParam, Void *);
			Int		 ModifyListEntry(Int, String, ProcParam, Void *);
			Int		 RemoveListEntry(Int);

			Void		 GetSize();
			Void		 GetListEntriesSize();
		public:
			Array<Entry *>	 entries;

					 List();
					~List();

			Void		 CleanupList();
			Int		 GetNOfEntries();
			Int		 SelectListEntry(Int);
			Int		 GetSelectedEntry();
			String		 GetSelectedEntryName();
	};
};

#endif
