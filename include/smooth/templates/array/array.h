 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARRAY_
#define _H_OBJSMOOTH_ARRAY_

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

			Int		 lastN;
			Array_Entry<t>	*lastNthEntry;

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

			Bool		 RemoveEntry(Int);
			Bool		 RemoveAll();

			t		 GetEntry(Int);
			Bool		 SetEntry(Int, t);

			Int		 GetNOfEntries() const		{ return nOfEntries; };
			t		 GetFirstEntry();
			t		 GetLastEntry();
			t		 GetNextEntry();
			t		 GetPrevEntry();
			t		 GetNthEntry(Int);
			Int		 GetNthEntryIndex(Int);

		// static methods for accessing const arrays:

			static t	 GetNthEntry(const Array<t> *, Int);
	};
};

#endif
