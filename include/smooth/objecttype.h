 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_OBJECTTYPE_
#define _H_OBJSMOOTH_OBJECTTYPE_

namespace smooth
{
	class Object;
	class ObjectType;
};

#include "definitions.h"

namespace smooth
{
	class SMOOTHAPI ObjectType
	{
		friend class SMOOTHAPI Object;
		private:
			Int		 type;
			Object		*object;
		public:
					 ObjectType();
					 ObjectType(const ObjectType &);

			operator	 Int();
			Int operator	 =(Int);

			Bool operator	 ==(Int);
			Bool operator	 !=(Int);
	};
};

#endif
