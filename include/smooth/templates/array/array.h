 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_ARRAY_
#define _H_SMOOTHARRAY_ARRAY_

#include "entry.h"

namespace smooth
{
	class Array_Base
	{
		public:
			virtual	~Array_Base()
			{
			}
	};

	template <class t> class Array : public Array_Base
	{
		private:
			Int		 nOfEntries;
			Int		 greatestIndex;
			Bool		 outlinedEntry;

			Array_Entry<t>	*firstEntry;
			Array_Entry<t>	*lastEntry;
			Array_Entry<t>	*prevEntry;
			Array_Entry<t>	*prevDeletedEntry;

			Bool		 Cleanup(Array_Entry<t> *);
			Bool		 IndexAvailable(Int);
		public:
					 Array();
					~Array();

			Int		 AddEntry(t);
			Bool		 AddEntry(t, Int);

			Int		 InsertEntryAfter(Int, t);
			Bool		 InsertEntryAfter(Int, t, Int);
			Int		 InsertEntryAtPos(Int, t);
			Bool		 InsertEntryAtPos(Int, t, Int);

			Bool		 DeleteEntry(Int);
			Bool		 DeleteAll();

			t		 GetEntry(Int);
			Bool		 SetEntry(Int, t);

			Int		 GetNOfEntries();
			t		 GetFirstEntry();
			t		 GetLastEntry();
			t		 GetNextEntry();
			t		 GetPrevEntry();
			t		 GetNthEntry(Int);
			Int		 GetNthEntryIndex(Int);

			static Int	 GetNOfEntries(const Array<t> *);
			static t	 GetNthEntry(const Array<t> *, Int);
	};
};

#endif
