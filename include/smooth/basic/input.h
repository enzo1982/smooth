 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_INPUT_
#define _H_INPUT_

namespace smooth
{
	class Input;
};

#include "../definitions.h"

namespace smooth
{
	class SMOOTHAPI Input
	{
		private:
					 Input();
					 Input(const Input &);
		public:
			static Int	 MouseX();
			static Int	 MouseY();
	};
};

#endif
