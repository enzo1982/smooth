 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/haiku/pointerhaiku.h>
#include <smooth/backends/haiku/backendhaiku.h>

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
	BApplication	*app = Backends::BackendHaiku::GetApplication();

	if	(mouseCursor == Pointer::CursorArrow)	 app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorTextEdit) app->SetCursor(B_CURSOR_I_BEAM);
	else if (mouseCursor == Pointer::CursorHand)	 app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorHSize)	 app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else if (mouseCursor == Pointer::CursorVSize)	 app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
	else						 return False;

	return True;
}
