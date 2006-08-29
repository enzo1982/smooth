 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#include "backend.h"

namespace smooth
{
	template <class t, class s = t> class Array : public Array_Backend<s>
	{
		public:
					 Array()							{ }
			virtual		~Array()							{ }

			Int		 AddEntry(const t &value)					{ return Array_Backend<s>::AddEntry((s) value); }
			Bool		 AddEntry(const t &value, Int index)				{ return Array_Backend<s>::AddEntry((s) value, index); }

			Int		 InsertEntryAfter(Int index, const t &value)			{ return Array_Backend<s>::InsertEntryAfter(index, (s) value); }
			Bool		 InsertEntryAfter(Int index, const t &value, Int nIndex)	{ return Array_Backend<s>::InsertEntryAfter(index, (s) value, nIndex); }
			Int		 InsertEntryAtPos(Int pos, const t &value)			{ return Array_Backend<s>::InsertEntryAtPos(pos, (s) value); }
			Bool		 InsertEntryAtPos(Int pos, const t &value, Int nIndex)		{ return Array_Backend<s>::InsertEntryAtPos(pos, (s) value, nIndex); }

			const t		&GetEntry(Int index) const					{ return (const t &) Array_Backend<s>::GetEntry(index); }
			Bool		 SetEntry(Int index, const t &value)				{ return Array_Backend<s>::SetEntry(index, (s) value); }

			const t		&GetFirstEntry() const						{ return (const t &) Array_Backend<s>::GetFirstEntry(); }
			const t		&GetLastEntry() const						{ return (const t &) Array_Backend<s>::GetLastEntry(); }
			const t		&GetNextEntry() const						{ return (const t &) Array_Backend<s>::GetNextEntry(); }
			const t		&GetPrevEntry() const						{ return (const t &) Array_Backend<s>::GetPrevEntry(); }
			const t		&GetNthEntry(Int n) const					{ return (const t &) Array_Backend<s>::GetNthEntry(n); }
	};
};

#endif
