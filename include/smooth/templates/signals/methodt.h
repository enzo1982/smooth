 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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
	};
};

#endif
