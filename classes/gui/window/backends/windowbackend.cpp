 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/backends/windowbackend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/gui/window/backends/gdi/windowgdi.h>
#endif

S::GUI::WindowBackend *CreateWindowBackend()
{
	return new S::GUI::WindowBackend();
}

S::Int	 windowBackendTmp = S::GUI::WindowBackend::AddBackend(&CreateWindowBackend);

S::Array<S::GUI::WindowBackend *(*)(), S::Void *>	*S::GUI::WindowBackend::backend_creators = NIL;

S::Int S::GUI::WindowBackend::AddBackend(WindowBackend *(*backend)())
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<WindowBackend *(*)(), Void *>;

	backend_creators->Add(backend);

	return Success();
}

S::GUI::WindowBackend *S::GUI::WindowBackend::CreateBackendInstance()
{
	if (backend_creators->GetFirst() != &CreateWindowBackend)	return backend_creators->GetFirst()();
	else								return backend_creators->GetLast()();
}

S::GUI::WindowBackend::WindowBackend(Void *iWindow)
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) WindowGDI();
#endif

	type = WINDOW_NONE;

	nullSurface = new Surface();
	drawSurface = nullSurface;
}

S::GUI::WindowBackend::~WindowBackend()
{
	delete nullSurface;
}

S::Int S::GUI::WindowBackend::GetWindowType()
{
	return type;
}

S::Void *S::GUI::WindowBackend::GetSystemWindow() const
{
	return NIL;
}

S::GUI::Surface *S::GUI::WindowBackend::GetDrawSurface() const
{
	return drawSurface;
}

S::Int S::GUI::WindowBackend::Open(const String &title, const Point &pos, const Size &size, Int flags)
{
	return Success();
}

S::Int S::GUI::WindowBackend::Close()
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetTitle(const String &nTitle)
{
	return Success();
}

S::Int S::GUI::WindowBackend::SetIcon(const Bitmap &newIcon)
{
	return Success();
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
