 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_ARRAY_ENTRY_
#define _H_OBJSMOOTH_ARRAY_ENTRY_

#include "../../definitions.h"

namespace smooth
{
	template <class s> class Array_Entry
	{
		private:
			s			 value;
			Int			 index;

			Array_Entry<s>		*nextEntry;
			Array_Entry<s>		*prevEntry;
		public:
						 Array_Entry(const s &iValue)		{ value = iValue; index = 0; nextEntry = ARRAY_NULLPOINTER; prevEntry = ARRAY_NULLPOINTER; }
			virtual			~Array_Entry()				{ }
		accessors:
			inline const s		&GetValue() const			{ return value; }
			Bool			 SetValue(const s &nValue)		{ value = nValue; return True; }

			inline Int		 GetIndex() const			{ return index; }
			Bool			 SetIndex(Int nIndex)			{ index = nIndex; return True; }

			inline Array_Entry<s>	*GetNext() const			{ return nextEntry; }
			Bool			 SetNext(Array_Entry<s> *entry)		{ nextEntry = entry; return True; }

			inline Array_Entry<s>	*GetPrev() const			{ return prevEntry; }
			Bool			 SetPrev(Array_Entry<s> *entry)		{ prevEntry = entry; return True; }
	};
};

#endif
