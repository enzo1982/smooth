 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_ENTRY_
#define _H_SMOOTHARRAY_ENTRY_

#include "value.h"

namespace smooth
{
	template <class t> class Array_Entry : public Array_Value<t>
	{
		private:
			Array_Entry<t>	*nextEntry;
			Array_Entry<t>	*prevEntry;
		public:
			Bool			 gotNext;
			Bool			 gotPrev;

						 Array_Entry();
						~Array_Entry();

			Bool			 SetNext(Array_Entry<t> *);
			Bool			 SetPrev(Array_Entry<t> *);
			inline Array_Entry<t>	*GetNext();
			inline Array_Entry<t>	*GetPrev();

			static Array_Entry<t>	*GetNext(const Array_Entry<t> *);
			static Array_Entry<t>	*GetPrev(const Array_Entry<t> *);
	};
};

#endif
