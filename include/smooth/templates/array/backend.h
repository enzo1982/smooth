 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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
			static s		 nullValue;

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

			Int			 Add(const s &);
			Bool			 Add(const s &, Int);

			Int			 InsertAfter(Int, const s &);
			Bool			 InsertAfter(Int, const s &, Int);
			Int			 InsertAtPos(Int, const s &);
			Bool			 InsertAtPos(Int, const s &, Int);

			Bool			 Remove(Int);
			Bool			 RemoveAll();

			const s			&Get(Int) const;
			Bool			 Set(Int, const s &);

			inline Int		 GetNOfEntries() const		{ return nOfEntries; };
			const s			&GetFirst() const;
			const s			&GetLast() const;
			const s			&GetNext() const;
			const s			&GetPrev() const;
			const s			&GetNth(Int) const;
			Int			 GetNthIndex(Int) const;
	};
};

#endif
