 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SYSTEM_
#define _H_SYSTEM_

namespace smooth
{
	class System;
};

#include "definitions.h"
#include "string.h"

namespace smooth
{
	class SMOOTHAPI System
	{
		private:
			static Int	 nextGUID;

					 System();
					 System(const System &);
		public:
			static Int	 RequestGUID();

			static String	 GetVersionString();
	};
};

#endif
