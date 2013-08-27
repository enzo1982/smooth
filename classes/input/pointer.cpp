 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/pointer.h>
#include <smooth/input/backends/pointerbackend.h>
#include <smooth/init.h>

S::Int	 addPointerInitTmp = S::AddInitFunction(&S::Input::Pointer::Initialize);
S::Int	 addPointerFreeTmp = S::AddFreeFunction(&S::Input::Pointer::Free);

S::Input::PointerBackend	*S::Input::Pointer::backend = NIL;

const S::GUI::Window		*S::Input::Pointer::pointerWindow = NIL;
S::GUI::Point			 S::Input::Pointer::mousePosition = S::GUI::Point();

S::Input::Pointer::Pointer()
{
}

S::Input::Pointer::Pointer(const Pointer &)
{
}

S::Int S::Input::Pointer::Initialize()
{
	backend = PointerBackend::CreateBackendInstance();

	return Success();
}

S::Int S::Input::Pointer::Free()
{
	delete backend;

	return Success();
}

S::Bool S::Input::Pointer::SetCursor(const GUI::Window *window, CursorType mouseCursor)
{
	return backend->SetCursor(window, mouseCursor);
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
