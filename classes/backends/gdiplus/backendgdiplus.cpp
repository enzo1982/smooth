 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/gdiplus/backendgdiplus.h>
#include <smooth/backends/platform.h>

S::Backends::Backend *CreateBackendGDIPlus()
{
	return new S::Backends::BackendGDIPlus();
}

static S::Bool ProbeGDIPlus()
{
	return S::True;
}

S::Int	 backendGDIPlusTmp = S::Backends::Backend::AddBackend(&CreateBackendGDIPlus);
S::Int	 backendGDIPlusProbeTmp = S::Backends::Platform::AddProbe(S::Backends::PLATFORM_GDIPLUS, 1, &ProbeGDIPlus);

S::Backends::BackendGDIPlus::BackendGDIPlus()
{
	type = BACKEND_GDIPLUS;
}

S::Backends::BackendGDIPlus::~BackendGDIPlus()
{
}

S::Int S::Backends::BackendGDIPlus::Init()
{
	if (!Platform::IsSelected(PLATFORM_GDIPLUS)) return Success();

	Gdiplus::GdiplusStartupInput	 gdiplusStartupInput;

	/* Initialize GDI+.
	 */
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	return Success();
}

S::Int S::Backends::BackendGDIPlus::Deinit()
{
	if (!Platform::IsSelected(PLATFORM_GDIPLUS)) return Success();

	Gdiplus::GdiplusShutdown(gdiplusToken);

	return Success();
}
