 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CINSTANCE_
#define _H_CINSTANCE_

namespace smooth
{
	template <class rt> class CInstance
	{
		public:
			virtual			~CInstance() {}

			virtual CInstance<rt>	*Copy() = 0;
			virtual rt		 Call(CMethod *, ...) = 0;

			virtual Bool operator	 ==(CInstance<rt> *) = 0;
	};
};

#endif
