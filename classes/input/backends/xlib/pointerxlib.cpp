 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/xlib/pointerxlib.h>

using namespace X11;

S::Input::PointerBackend *CreatePointerXLib()
{
	return new S::Input::PointerXLib();
}

S::Int	 pointerXLibTmp = S::Input::PointerBackend::SetBackend(&CreatePointerXLib);

S::Input::PointerXLib::PointerXLib()
{
	type = POINTER_XLIB;

	display	= Backends::BackendXLib::GetDisplay();
}

S::Input::PointerXLib::~PointerXLib()
{
}

S::Bool S::Input::PointerXLib::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	static Cursor	 hCursorTextEdit = XCreateFontCursor(display, XC_xterm);
	static Cursor	 hCursorHand	 = XCreateFontCursor(display, XC_hand2);
	static Cursor	 hCursorHSize	 = XCreateFontCursor(display, XC_sb_h_double_arrow);
	static Cursor	 hCursorVSize	 = XCreateFontCursor(display, XC_sb_v_double_arrow);

	if	(mouseCursor == Pointer::CursorArrow)	 XUndefineCursor(display, (Window) window->GetSystemWindow());
	else if (mouseCursor == Pointer::CursorTextEdit) XDefineCursor(display, (Window) window->GetSystemWindow(), hCursorTextEdit);
	else if (mouseCursor == Pointer::CursorHand)	 XDefineCursor(display, (Window) window->GetSystemWindow(), hCursorHand);
	else if (mouseCursor == Pointer::CursorHSize)	 XDefineCursor(display, (Window) window->GetSystemWindow(), hCursorHSize);
	else if (mouseCursor == Pointer::CursorVSize)	 XDefineCursor(display, (Window) window->GetSystemWindow(), hCursorVSize);
	else						 return False;

	XFlush(display);

	return True;
}
