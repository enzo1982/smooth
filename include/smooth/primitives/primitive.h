 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_PRIMITIVE_
#define _H_OBJSMOOTH_PRIMITIVE_

namespace smooth
{
	class Primitive;

	namespace GUI
	{
		class Surface;
	};
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI Primitive
	{
		protected:
			UnsignedLong	 color;
		public:
					 Primitive();
			virtual		~Primitive();

			Int		 SetColor(UnsignedLong);
			UnsignedLong	 GetColor();

			virtual Int	 Draw(GUI::Surface *);
	};
};

#endif
