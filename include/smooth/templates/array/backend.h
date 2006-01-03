 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARRAY_BACKEND_
#define _H_OBJSMOOTH_ARRAY_BACKEND_

#include "entry.h"

namespace smooth
{
	template <class s> class Array_Backend
	{
		private:
			Int			 nOfEntries;
			Int			 greatestIndex;
			Bool			 outlinedEntry;

			Array_Entry<s>		*firstEntry;
			Array_Entry<s>		*lastEntry;

			mutable Array_Entry<s>	*prevAccessedEntry;

			mutable Int		 lastN;
			mutable Array_Entry<s>	*lastNthEntry;

			Bool			 IndexAvailable(Int) const;
			Array_Entry<s>		*GetArrayEntryByIndex(Int) const;
			Array_Entry<s>		*GetNthArrayEntry(Int) const;
		public:
						 Array_Backend();
			virtual			~Array_Backend();

			Int			 AddEntry(const s &);
			Bool			 AddEntry(const s &, Int);

			Int			 InsertEntryAfter(Int, const s &);
			Bool			 InsertEntryAfter(Int, const s &, Int);
			Int			 InsertEntryAtPos(Int, const s &);
			Bool			 InsertEntryAtPos(Int, const s &, Int);

			Bool			 RemoveEntry(Int);
			Bool			 RemoveAll();

			s			 GetEntry(Int) const;
			Bool			 SetEntry(Int, const s &);

			inline Int		 GetNOfEntries() const		{ return nOfEntries; };
			s			 GetFirstEntry() const;
			s			 GetLastEntry() const;
			s			 GetNextEntry() const;
			s			 GetPrevEntry() const;
			s			 GetNthEntry(Int) const;
			Int			 GetNthEntryIndex(Int) const;
	};
};

#endif
