 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/input/backends/pointerbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/input/backends/win32/pointerwin32.h>
#endif

S::Input::PointerBackend *CreatePointerBackend()
{
	return new S::Input::PointerBackend();
}

S::Input::PointerBackend *(*S::Input::PointerBackend::backend_creator)() = &CreatePointerBackend;

S::Int S::Input::PointerBackend::SetBackend(PointerBackend *(*backend)())
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::Input::PointerBackend *S::Input::PointerBackend::CreateBackendInstance()
{
	return backend_creator();
}

S::Input::PointerBackend::PointerBackend()
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) PointerWin32();
#endif

	type = POINTER_NONE;
}

S::Input::PointerBackend::~PointerBackend()
{
}

S::Short S::Input::PointerBackend::GetPointerType() const
{
	return type;
}

S::Bool S::Input::PointerBackend::SetCursor(const GUI::Window *window, Pointer::CursorType mouseCursor)
{
	return False;
}
