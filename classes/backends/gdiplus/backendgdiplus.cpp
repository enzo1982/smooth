 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/gdiplus/backendgdiplus.h>

S::Backends::Backend *CreateBackendGDIPlus()
{
	return new S::Backends::BackendGDIPlus();
}

S::Int	 backendGDIPlusTmp = S::Backends::Backend::AddBackend(&CreateBackendGDIPlus);

S::Backends::BackendGDIPlus::BackendGDIPlus()
{
	type = BACKEND_GDIPLUS;
}

S::Backends::BackendGDIPlus::~BackendGDIPlus()
{
}

S::Int S::Backends::BackendGDIPlus::Init()
{
	Gdiplus::GdiplusStartupInput	 gdiplusStartupInput;

	/* Initialize GDI+.
	 */
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	return Success();
}

S::Int S::Backends::BackendGDIPlus::Deinit()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return Success();
}
