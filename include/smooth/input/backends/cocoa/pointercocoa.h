 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_POINTERCOCOA
#define H_OBJSMOOTH_POINTERCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace System
	{
		class PointerCocoa;
	};
};

#include "../pointerbackend.h"

namespace smooth
{
	namespace Input
	{
		const Short	 POINTER_COCOA = 3;

		class PointerCocoa : public PointerBackend
		{
			public:
					 PointerCocoa();
					~PointerCocoa();

				Bool	 SetCursor(const GUI::Window *, Pointer::CursorType);
		};
	};
};

#endif
