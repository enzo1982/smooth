 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_PRIMITIVE_
#define _H_OBJSMOOTH_PRIMITIVE_

namespace smooth
{
	class Primitive;
	class Drawable;
};

#include "definitions.h"

namespace smooth
{
	class SMOOTHAPI Primitive
	{
		public:
					 Primitive();
			virtual		~Primitive();

			virtual Int	 Draw(Drawable *);
	};
};

#endif
