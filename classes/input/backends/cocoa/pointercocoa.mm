 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/cocoa/pointercocoa.h>

S::Input::PointerBackend *CreatePointerCocoa()
{
	return new S::Input::PointerCocoa();
}

S::Int	 pointerCocoaTmp = S::Input::PointerBackend::SetBackend(&CreatePointerCocoa);

S::Input::PointerCocoa::PointerCocoa()
{
	type = POINTER_COCOA;
}

S::Input::PointerCocoa::~PointerCocoa()
{
}

S::Bool S::Input::PointerCocoa::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	if	(mouseCursor == Pointer::CursorArrow)	 [[NSCursor arrowCursor]	   set];
	else if (mouseCursor == Pointer::CursorTextEdit) [[NSCursor IBeamCursor]	   set];
	else if (mouseCursor == Pointer::CursorHand)	 [[NSCursor pointingHandCursor]	   set];
	else if (mouseCursor == Pointer::CursorHSize)	 [[NSCursor resizeLeftRightCursor] set];
	else if (mouseCursor == Pointer::CursorVSize)	 [[NSCursor resizeUpDownCursor]	   set];
	else						 return False;

	return True;
}
