 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../signals/method.h"

#ifndef _H_METHODT_
#define _H_METHODT_

namespace smooth
{
	template <class t> class MethodT : public Method
	{
		public:
			t	 method;
				 MethodT(t iMethod)	{ method = iMethod; }
			Method	*Copy()			{ return new MethodT<t>(method); }

			Bool operator ==(Method *cMethod)
			{
				if (method == ((MethodT<t> *) cMethod)->method)	return True;
				else						return False;
			}
	};
};

#endif
