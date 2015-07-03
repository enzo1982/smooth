 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/windowbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/gui/window/backends/gdi/windowgdi.h>
#endif

S::GUI::WindowBackend *CreateWindowBackend()
{
	return new S::GUI::WindowBackend();
}

S::GUI::WindowBackend *(*S::GUI::WindowBackend::backend_creator)() = &CreateWindowBackend;

S::Int S::GUI::WindowBackend::SetBackend(WindowBackend *(*backend)())
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::GUI::WindowBackend *S::GUI::WindowBackend::CreateBackendInstance()
{
	return backend_creator();
}

S::GUI::WindowBackend::WindowBackend(Void *iWindow)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) WindowGDI();
#endif

	type = WINDOW_NONE;

	drawSurface = NIL;

	doClose.Connect(True);
}

S::GUI::WindowBackend::~WindowBackend()
{
}

S::Short S::GUI::WindowBackend::GetWindowType() const
{
	return type;
}

S::Void *S::GUI::WindowBackend::GetSystemWindow() const
{
	return NIL;
}

S::GUI::Surface *S::GUI::WindowBackend::GetDrawSurface() const
{
	if (drawSurface != NIL) return drawSurface;
	else			return Surface::GetNullSurface();
}

S::Int S::GUI::WindowBackend::Open(const String &title, const Point &pos, const Size &size, Int flags)
{
	return Success();
}

S::Int S::GUI::WindowBackend::Close()
{
	return Success();
}

S::Int S::GUI::WindowBackend::RequestClose()
{
	return Close();
}

S::Int S::GUI::WindowBackend::SetTitle(const String &nTitle)
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetIcon(const Bitmap &newIcon)
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetIconDirect(Void *newIcon)
{
	return Success();
}

S::Int S::GUI::WindowBackend::EnableDropFiles(Bool nEnabled)
{
	return Success();
}

const S::Array<S::String> &S::GUI::WindowBackend::GetDroppedFiles() const
{
	static Array<String>	 none;

	return none;
}

S::Int S::GUI::WindowBackend::SetMinimumSize(const Size &nMinSize)
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetMaximumSize(const Size &nMaxSize)
{
	return Success();
}

S::Int S::GUI::WindowBackend::Show()
{
	return Success();
}

S::Int S::GUI::WindowBackend::Hide()
{
	return Success();
}

S::GUI::Rect S::GUI::WindowBackend::GetRestoredWindowRect() const
{
	return Rect();
}

S::Int S::GUI::WindowBackend::SetMetrics(const Point &nPos, const Size &nSize)
{
	return Success();
}

S::Int S::GUI::WindowBackend::Minimize()
{
	return Success();
}

S::Int S::GUI::WindowBackend::Maximize()
{
	return Success();
}

S::Int S::GUI::WindowBackend::Restore()
{
	return Success();
}

S::Int S::GUI::WindowBackend::Raise()
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetProgressIndicator(Window::ProgressIndicatorState state, Float value)
{
	return Success();
}

const S::GUI::Size &S::GUI::WindowBackend::GetSizeModifier() const
{
	return sizeModifier;
}

S::Void S::GUI::WindowBackend::SetSizeModifier(const Size &nSizeModifier)
{
	sizeModifier = nSizeModifier;
}

const S::GUI::Rect &S::GUI::WindowBackend::GetUpdateRect() const
{
	return updateRect;
}
