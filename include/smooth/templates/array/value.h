 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SMOOTHARRAY_VALUE_
#define _H_SMOOTHARRAY_VALUE_

#include "../../definitions.h"

namespace smooth
{
	class Array_Value_Base
	{
		public:
			virtual	~Array_Value_Base()
			{
			}
	};

	template <class t> class Array_Value : public Array_Value_Base
	{
		private:
			Bool		 filled;
			t		 value;
			Int		 index;
		public:
					 Array_Value();
					~Array_Value();

			Bool		 Fill(t);
			Bool		 Clear();
			inline t	 GetValue();
			inline Int	 GetIndex();
			Bool		 SetIndex(Int);

			static t	 GetValue(const Array_Value<t> *);
			static Int	 GetIndex(const Array_Value<t> *);
	};
};

#endif
