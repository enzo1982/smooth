 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_POINTERBACKEND
#define H_OBJSMOOTH_POINTERBACKEND

namespace smooth
{
	namespace System
	{
		class Pointer;
		class PointerBackend;
	};
};

#include "../../input/pointer.h"
#include "../../gui/window/window.h"

namespace smooth
{
	namespace Input
	{
		const Short	POINTER_NONE = 0;

		class PointerBackend
		{
			private:
				static PointerBackend	*(*backend_creator)();
			protected:
				Short			 type;
			public:
				static Int		 SetBackend(PointerBackend *(*)());
				static PointerBackend	*CreateBackendInstance();

							 PointerBackend();
				virtual			~PointerBackend();

				Short			 GetPointerType() const;

				virtual Bool		 SetCursor(const GUI::Window *, Pointer::CursorType);
		};
	};
};

#endif
