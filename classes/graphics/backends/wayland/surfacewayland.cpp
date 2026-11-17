 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/wayland/surfacewayland.h>
#include <smooth/backends/platform.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/window/backends/wayland/windowwayland.h>
#include <smooth/backends/wayland/backendwayland.h>
#include <smooth/misc/math.h>

S::GUI::SurfaceBackend *CreateSurfaceWayland(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceWayland(iSurface, maxSize);
}

static S::Int Register_surfaceWaylandTmp() { return S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceWayland); }

S::Int		 surfaceWaylandTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_WAYLAND, &Register_surfaceWaylandTmp);

S::Short	 S::GUI::SurfaceWayland::surfaceDPI = -1;

S::GUI::SurfaceWayland::SurfaceWayland(Void *iWindow, const Size &maxSize)
{
	type	     = SURFACE_WAYLAND;

	window	     = (WindowWayland *) iWindow;

	if (window != NIL)
	{
		size = maxSize;

		if (size.cx <= 0) size.cx = 1;
		if (size.cy <= 0) size.cy = 1;

		fontSize.SetFontSize(GetSurfaceDPI());
		rightToLeft.SetSurfaceSize(size);

		CreateBackbuffer(size);
	}
}

S::GUI::SurfaceWayland::~SurfaceWayland()
{
}

cairo_surface_t *S::GUI::SurfaceWayland::CreateBackbufferSurface(const Size &nSize)
{
	return cairo_image_surface_create(CAIRO_FORMAT_RGB24, nSize.cx, nSize.cy);
}

S::Int S::GUI::SurfaceWayland::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL) window->Present(paintSurface, Rect::OverlapRect(pRect, Rect(Point(0, 0), size)));

	return Success();
}

S::Void *S::GUI::SurfaceWayland::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceWayland::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	/* Use the integer output scale, as we render
	 * at native resolution using buffer scaling.
	 */
	Float	 dpi = 96.0 * Backends::BackendWayland::GetScale();

	surfaceDPI = Math::Round(dpi * Setup::FontSize);

	return surfaceDPI;
}
