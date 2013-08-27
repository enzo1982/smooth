 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/win32/pointerwin32.h>

S::Input::PointerBackend *CreatePointerWin32()
{
	return new S::Input::PointerWin32();
}

S::Int	 pointerWin32Tmp = S::Input::PointerBackend::SetBackend(&CreatePointerWin32);

S::Input::PointerWin32::PointerWin32()
{
	type = POINTER_WIN32;
}

S::Input::PointerWin32::~PointerWin32()
{
}

S::Bool S::Input::PointerWin32::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	static HCURSOR	 hCursorArrow	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorTextEdit = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32513), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorHand	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32649), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorHSize	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32644), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorVSize	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32645), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	if (hCursorHand == NIL) hCursorHand = hCursorArrow;

	if (mouseCursor != Pointer::CursorArrow) SetClassLongPtrA((HWND) window->GetSystemWindow(), GCLP_HCURSOR, (LONG_PTR) NIL);
	else					 SetClassLongPtrA((HWND) window->GetSystemWindow(), GCLP_HCURSOR, (LONG_PTR) hCursorArrow);

	if	(mouseCursor == Pointer::CursorArrow)	 ::SetCursor(hCursorArrow);
	else if	(mouseCursor == Pointer::CursorTextEdit) ::SetCursor(hCursorTextEdit);
	else if	(mouseCursor == Pointer::CursorHand)	 ::SetCursor(hCursorHand);
	else if	(mouseCursor == Pointer::CursorHSize)	 ::SetCursor(hCursorHSize);
	else if	(mouseCursor == Pointer::CursorVSize)	 ::SetCursor(hCursorVSize);
	else						 return False;

	return True;
}
