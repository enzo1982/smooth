 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../callbacks/method.h"

#ifndef _H_CMETHODT_
#define _H_CMETHODT_

namespace smooth
{
	template <class t> class CMethodT : public CMethod
	{
		public:
			t	 method;
				 CMethodT(t iMethod)	{ method = iMethod; }
			CMethod	*Copy()			{ return new CMethodT<t>(method); }
	};
};

#endif
