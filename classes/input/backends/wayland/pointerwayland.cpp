 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/wayland/pointerwayland.h>
#include <smooth/backends/platform.h>
#include <smooth/gui/window/backends/wayland/windowwayland.h>

S::Input::PointerBackend *CreatePointerWayland()
{
	return new S::Input::PointerWayland();
}

static S::Int Register_pointerWaylandTmp() { return S::Input::PointerBackend::SetBackend(&CreatePointerWayland); }

S::Int	 pointerWaylandTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_WAYLAND, &Register_pointerWaylandTmp);

S::Input::PointerWayland::PointerWayland()
{
	type = POINTER_WAYLAND;
}

S::Input::PointerWayland::~PointerWayland()
{
}

S::Bool S::Input::PointerWayland::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	GUI::WindowWayland	*backend = GUI::WindowWayland::GetWindowBackend(window);

	if (backend == NIL) return False;

	if (mouseCursor != Pointer::CursorArrow	   &&
	    mouseCursor != Pointer::CursorTextEdit &&
	    mouseCursor != Pointer::CursorHand	   &&
	    mouseCursor != Pointer::CursorHSize	   &&
	    mouseCursor != Pointer::CursorVSize) return False;

	backend->SetPointerCursor(mouseCursor);

	return True;
}
