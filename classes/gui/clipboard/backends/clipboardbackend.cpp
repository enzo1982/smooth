 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/clipboardbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/gui/clipboard/backends/win32/clipboardwin32.h>
#endif

S::GUI::ClipboardBackend *CreateClipboardBackend(S::GUI::Window *window)
{
	return new S::GUI::ClipboardBackend();
}

S::GUI::ClipboardBackend *(*S::GUI::ClipboardBackend::backend_creator)(Window *) = &CreateClipboardBackend;

S::Int S::GUI::ClipboardBackend::SetBackend(ClipboardBackend *(*backend)(Window *))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::GUI::ClipboardBackend *S::GUI::ClipboardBackend::CreateBackendInstance(Window *window)
{
	return backend_creator(window);
}

S::GUI::ClipboardBackend::ClipboardBackend()
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) ClipboardWin32(NIL);
#endif

	type = CLIPBOARD_NONE;
}

S::GUI::ClipboardBackend::~ClipboardBackend()
{
}

S::Short S::GUI::ClipboardBackend::GetClipboardType() const
{
	return type;
}

S::String S::GUI::ClipboardBackend::GetClipboardText() const
{
	return NIL;
}

S::Bool S::GUI::ClipboardBackend::SetClipboardText(const String &text)
{
	return False;
}
