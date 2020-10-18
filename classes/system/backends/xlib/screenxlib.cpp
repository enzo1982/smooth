 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/xlib/screenxlib.h>
#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/input/pointer.h>
#include <smooth/misc/math.h>
#include <smooth/init.h>

#include <dlfcn.h>

using namespace X11;

S::System::ScreenBackend *CreateScreenXLib()
{
	return new S::System::ScreenXLib();
}

S::Int	 screenXLibTmp = S::System::ScreenBackend::SetBackend(&CreateScreenXLib);

S::Int	 addScreenXLibInitTmp = S::AddInitFunction(&S::System::ScreenXLib::Initialize);
S::Int	 addScreenXLibFreeTmp = S::AddFreeFunction(&S::System::ScreenXLib::Free);

namespace X11
{
#	define Bool int

	typedef XID	 RROutput;

	struct XRRMonitorInfo
	{
		Atom		 name;
		Bool		 primary;
		Bool		 automatic;
		int		 noutput;
		int		 x;
		int		 y;
		int		 width;
		int		 height;
		int		 mwidth;
		int		 mheight;
		RROutput	*outputs;
	};

	static void			*libXrandr = NIL;

	typedef XRRMonitorInfo *	(*xrrgetmonitors_t)(Display *, Window, Bool, int *);
	typedef void			(*xrrfreemonitors_t)(XRRMonitorInfo *);

	static xrrgetmonitors_t		 XRRGetMonitors	 = NIL;
	static xrrfreemonitors_t	 XRRFreeMonitors = NIL;

#	undef Bool
};

namespace smooth
{
	GUI::Rect MonitorRectFromPoint(Display *display, const GUI::Point &point)
	{
		if (XRRGetMonitors == NIL) return GUI::Rect();

		GUI::Rect	 metrics;

		int		 monitorCount = 0;
		XRRMonitorInfo	*monitors     = XRRGetMonitors(display, DefaultRootWindow(display), True, &monitorCount);

		for (Int i = 0; i < monitorCount; i++)
		{
			if (point.x >= monitors[i].x			 &&
			    point.x <  monitors[i].x + monitors[i].width &&
			    point.y >= monitors[i].y			 &&
			    point.y <  monitors[i].y + monitors[i].height) metrics = GUI::Rect(GUI::Point(monitors[i].x, monitors[i].y), GUI::Size(monitors[i].width, monitors[i].height));
		}

		XRRFreeMonitors(monitors);

		return metrics;
	}
};

S::Int S::System::ScreenXLib::Initialize()
{
	libXrandr = dlopen("libXrandr.so", RTLD_NOW | RTLD_LOCAL);

	if (libXrandr != NIL)
	{
		XRRGetMonitors	= (xrrgetmonitors_t) dlsym(libXrandr, "XRRGetMonitors");
		XRRFreeMonitors	= (xrrfreemonitors_t) dlsym(libXrandr, "XRRFreeMonitors");
	}

	return Success();
}

S::Int S::System::ScreenXLib::Free()
{
	if (libXrandr != NIL) dlclose(libXrandr);

	return Success();
}

S::System::ScreenXLib::ScreenXLib()
{
	type = SCREEN_XLIB;

	display	= Backends::BackendXLib::GetDisplay();
}

S::System::ScreenXLib::~ScreenXLib()
{
}

S::GUI::Rect S::System::ScreenXLib::GetActiveScreenMetrics()
{
	if (display == NIL) return GUI::Rect();

	if (XRRGetMonitors != NIL)
	{
		GUI::Rect	 metrics = MonitorRectFromPoint(display, Input::Pointer::GetPosition());

		if (metrics != GUI::Rect()) return metrics;
	}

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}

S::GUI::Rect S::System::ScreenXLib::GetActiveScreenWorkArea()
{
	if (display == NIL) return GUI::Rect();

	if (XRRGetMonitors != NIL)
	{
		GUI::Rect	 metrics = MonitorRectFromPoint(display, Input::Pointer::GetPosition());

		if (metrics != GUI::Rect()) return metrics;
	}

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}

S::GUI::Rect S::System::ScreenXLib::GetVirtualScreenMetrics()
{
	if (display == NIL) return GUI::Rect();

	if (XRRGetMonitors != NIL)
	{
		GUI::Rect	 metrics;

		int		 monitorCount = 0;
		XRRMonitorInfo	*monitors     = XRRGetMonitors(display, DefaultRootWindow(display), True, &monitorCount);

		for (Int i = 0; i < monitorCount; i++)
		{
			metrics.left   = Math::Min(metrics.left, monitors[i].x);
			metrics.top    = Math::Min(metrics.top, monitors[i].y);
			metrics.right  = Math::Max(metrics.right, monitors[i].x + monitors[i].width);
			metrics.bottom = Math::Max(metrics.bottom, monitors[i].y + monitors[i].height);
		}

		XRRFreeMonitors(monitors);

		if (monitorCount > 0) return metrics;
	}

	return GUI::Rect(GUI::Point(0, 0), GUI::Size(XWidthOfScreen(XDefaultScreenOfDisplay(display)), XHeightOfScreen(XDefaultScreenOfDisplay(display))));
}
