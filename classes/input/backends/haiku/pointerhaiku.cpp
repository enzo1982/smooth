 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/haiku/pointerhaiku.h>

S::Input::PointerBackend *CreatePointerHaiku()
{
	return new S::Input::PointerHaiku();
}

S::Int	 pointerHaikuTmp = S::Input::PointerBackend::SetBackend(&CreatePointerHaiku);

S::Input::PointerHaiku::PointerHaiku()
{
	type = POINTER_HAIKU;
}

S::Input::PointerHaiku::~PointerHaiku()
{
}

S::Bool S::Input::PointerHaiku::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	if	(mouseCursor == Pointer::CursorArrow)	 be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorTextEdit) be_app->SetCursor(B_CURSOR_I_BEAM);
	else if (mouseCursor == Pointer::CursorHand)	 be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorHSize)	 be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorVSize)	 be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else						 return False;

	return True;
}
