 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
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
	class Array_Entry_Base
	{
		public:
			virtual	~Array_Entry_Base()
			{
			}
	};

	template <class t> class Array_Entry : public Array_Entry_Base
	{
		private:
			Bool			 filled;
			t			 value;
			Int			 index;

			Array_Entry<t>		*nextEntry;
			Array_Entry<t>		*prevEntry;
		public:
						 Array_Entry();
						~Array_Entry();

			Bool			 Fill(t);
			Bool			 Clear();

			t			 GetValue() const			{ return value; }

			Int			 GetIndex() const			{ return index; }
			Bool			 SetIndex(Int);

			Array_Entry<t>		*GetNext() const			{ return nextEntry; }
			Bool			 SetNext(Array_Entry<t> *entry)		{ nextEntry = entry; return True; }

			Array_Entry<t>		*GetPrev() const			{ return prevEntry; }
			Bool			 SetPrev(Array_Entry<t> *entry)		{ prevEntry = entry; return True; }
	};
};

#endif
