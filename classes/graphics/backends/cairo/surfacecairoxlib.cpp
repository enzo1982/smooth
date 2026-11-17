 /* The smooth Class Library
  * Copyright (C) 1998-2026 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gdk/gdk.h>

#include <smooth/graphics/backends/cairo/surfacecairoxlib.h>
#include <smooth/backends/platform.h>
#include <smooth/gui/application/application.h>
#include <smooth/system/dynamicloader.h>
#include <smooth/files/file.h>
#include <smooth/misc/math.h>
#include <smooth/misc/number.h>

using namespace X11;

#include <cairo/cairo-xlib.h>

#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/backends/freedesktop/desktopsettings.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

S::GUI::SurfaceBackend *CreateSurfaceCairoXLib(S::Void *iSurface, const S::GUI::Size &maxSize)
{
	return new S::GUI::SurfaceCairoXLib(iSurface, maxSize);
}

static S::Int Register_surfaceCairoXLibTmp() { return S::GUI::SurfaceBackend::SetBackend(&CreateSurfaceCairoXLib); }

S::Int		 surfaceCairoXLibTmp = S::Backends::Platform::AddComponent(S::Backends::PLATFORM_X11, &Register_surfaceCairoXLibTmp);

/* Free backbuffer pixmaps along with their Cairo surfaces.
 */
static cairo_user_data_key_t	 pixmapKey;

static void FreePixmap(void *pixmap)
{
	XFreePixmap(S::Backends::BackendXLib::GetDisplay(), (Pixmap) pixmap);
}

S::Short	 S::GUI::SurfaceCairoXLib::surfaceDPI = -1;

S::GUI::SurfaceCairoXLib::SurfaceCairoXLib(Void *iWindow, const Size &maxSize)
{
	type	     = SURFACE_CAIRO;

	window	     = (X11::Window) iWindow;

	display	     = Backends::BackendXLib::GetDisplay();

	if (display != NIL) visual = XDefaultVisual(display, XDefaultScreen(display));
	else		    visual = NIL;

	if (window != NIL)
	{
		size = maxSize;

		if (maxSize == Size())
		{
			size.cx = XDisplayWidth(display, XDefaultScreen(display)) + 2;
			size.cy = XDisplayHeight(display, XDefaultScreen(display)) + 2;
		}

		fontSize.SetFontSize(GetSurfaceDPI());
		rightToLeft.SetSurfaceSize(size);

		CreateBackbuffer(size);
	}
}

S::GUI::SurfaceCairoXLib::~SurfaceCairoXLib()
{
}

cairo_surface_t *S::GUI::SurfaceCairoXLib::CreateBackbufferSurface(const Size &nSize)
{
	XWindowAttributes	 windowAttributes;

	XGetWindowAttributes(display, window, &windowAttributes);

	Pixmap		 pixmap	 = XCreatePixmap(display, DefaultRootWindow(display), nSize.cx, nSize.cy, windowAttributes.depth);
	cairo_surface_t	*surface = cairo_xlib_surface_create(display, pixmap, visual, nSize.cx, nSize.cy);

	cairo_surface_set_user_data(surface, &pixmapKey, (void *) pixmap, &FreePixmap);

	return surface;
}

S::Int S::GUI::SurfaceCairoXLib::PaintRect(const Rect &pRect)
{
	if (painting) return Error();

	if (window != NIL)
	{
		cairo_surface_flush(paintSurface);

		GC	 gc = XCreateGC(display, window, 0, NIL);

		XCopyArea(display, cairo_xlib_surface_get_drawable(paintSurface), window, gc, pRect.left, pRect.top, pRect.GetWidth(), pRect.GetHeight(), pRect.left, pRect.top);

		XFreeGC(display, gc);
	}

	return Success();
}

S::Void *S::GUI::SurfaceCairoXLib::GetSystemSurface() const
{
	return (Void *) window;
}

S::Short S::GUI::SurfaceCairoXLib::GetSurfaceDPI() const
{
	if (Application::GetScaleFactor() != 0)	surfaceDPI = Math::Round(96.0 * Application::GetScaleFactor());

	if (surfaceDPI != -1) return surfaceDPI;

	Float	 dpi = 96.0;

	if (display != NIL)
	{
		/* Load gdk_screen_get_monitor_scale_factor dynamically.
		 */
		System::DynamicLoader	 gdk("gdk-3");
		gint			 (*ex_gdk_screen_get_monitor_scale_factor)(GdkScreen *, gint) = (gint (*)(GdkScreen *, gint)) gdk.GetFunctionAddress("gdk_screen_get_monitor_scale_factor");

		/* Get GDK scale factor.
		 */
		Float	 scale = 1.0;

		if (ex_gdk_screen_get_monitor_scale_factor != NIL) scale = ex_gdk_screen_get_monitor_scale_factor(gdk_screen_get_default(), 0);
		else						   scale = (Int64) Number::FromIntString(getenv("GDK_SCALE"));

		/* Alternatively, query the scale factor from the KDE configuration.
		 */
		if (scale <= 1.0) scale = Backends::DesktopSettings::QueryKDESettings("KScreen", "ScaleFactor").ToFloat();

		/* Alternatively, query custom DPI setting via XfConf.
		 */
		if (scale <= 1.0) scale = Backends::DesktopSettings::QueryXfConf("xsettings", "/Xft/DPI").ToInt() / dpi;

		/* Update DPI value if a scale factor has been determined.
		 */
		if (scale > 0) dpi *= scale;
	}

	surfaceDPI = Math::Round(dpi * Setup::FontSize);

	return surfaceDPI;
}
