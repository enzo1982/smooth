 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/pointer.h>
#include <smooth/gui/window/window.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <smooth/backends/xlib/backendxlib.h>

#	include <X11/cursorfont.h>

	using namespace X11;
#endif

const S::GUI::Window	*S::Input::Pointer::pointerWindow = NIL;
S::GUI::Point		 S::Input::Pointer::mousePosition = S::GUI::Point();

S::Input::Pointer::Pointer()
{
}

S::Input::Pointer::Pointer(const Pointer &)
{
}

S::Bool S::Input::Pointer::SetCursor(const GUI::Window *window, CursorType mouseCursor)
{
#ifdef __WIN32__
	static HCURSOR	 hCursorArrow	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32512), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorTextEdit = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32513), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorHand	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32649), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorHSize	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32644), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	static HCURSOR	 hCursorVSize	 = (HCURSOR) LoadImageA(NULL, MAKEINTRESOURCEA(32645), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	if (hCursorHand == NIL) hCursorHand = hCursorArrow;

	if (mouseCursor != CursorArrow)	SetClassLongPtrA((HWND) window->GetSystemWindow(), GCLP_HCURSOR, (LONG_PTR) NIL);
	else				SetClassLongPtrA((HWND) window->GetSystemWindow(), GCLP_HCURSOR, (LONG_PTR) hCursorArrow);

	if	(mouseCursor == CursorArrow)	::SetCursor(hCursorArrow);
	else if	(mouseCursor == CursorTextEdit)	::SetCursor(hCursorTextEdit);
	else if	(mouseCursor == CursorHand)	::SetCursor(hCursorHand);
	else if	(mouseCursor == CursorHSize)	::SetCursor(hCursorHSize);
	else if	(mouseCursor == CursorVSize)	::SetCursor(hCursorVSize);
	else					return False;
#else
	static Cursor	 hCursorTextEdit = XCreateFontCursor(Backends::BackendXLib::GetDisplay(), XC_xterm);
	static Cursor	 hCursorHand	 = XCreateFontCursor(Backends::BackendXLib::GetDisplay(), XC_hand2);
	static Cursor	 hCursorHSize	 = XCreateFontCursor(Backends::BackendXLib::GetDisplay(), XC_sb_h_double_arrow);
	static Cursor	 hCursorVSize	 = XCreateFontCursor(Backends::BackendXLib::GetDisplay(), XC_sb_v_double_arrow);

	if	(mouseCursor == CursorArrow)	XUndefineCursor(Backends::BackendXLib::GetDisplay(), (Window) window->GetSystemWindow());
	else if (mouseCursor == CursorTextEdit)	XDefineCursor(Backends::BackendXLib::GetDisplay(), (Window) window->GetSystemWindow(), hCursorTextEdit);
	else if (mouseCursor == CursorHand)	XDefineCursor(Backends::BackendXLib::GetDisplay(), (Window) window->GetSystemWindow(), hCursorHand);
	else if (mouseCursor == CursorHSize)	XDefineCursor(Backends::BackendXLib::GetDisplay(), (Window) window->GetSystemWindow(), hCursorHSize);
	else if (mouseCursor == CursorVSize)	XDefineCursor(Backends::BackendXLib::GetDisplay(), (Window) window->GetSystemWindow(), hCursorVSize);
	else					return False;

	XFlush(Backends::BackendXLib::GetDisplay());
#endif

	return True;
}

const S::GUI::Point &S::Input::Pointer::GetPosition()
{
	return mousePosition;
}

const S::GUI::Window *S::Input::Pointer::GetPointerWindow()
{
	return pointerWindow;
}

S::Void S::Input::Pointer::UpdatePosition(const GUI::Window *window, Int x, Int y)
{
	pointerWindow	= window;

	mousePosition.x	= x;
	mousePosition.y	= y;

	/* FixMe: The -2 offsets seem to be necessary on
	 *	  all X11 systems. However, I somehow feel
	 *	  this can't be the right way to do it.
	 */
#ifndef __WIN32__
	mousePosition.x -= 2;
	mousePosition.y -= 2;
#endif
}
