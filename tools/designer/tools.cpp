 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DESIGNER_TOOLS_
#define __OBJSMOOTH_DESIGNER_TOOLS_

#include <smoothx.h>
#include "tools.h"

SMOOTHInt MouseX(HWND hwnd, SMOOTHInt wincl)
{
	POINT	 point;
	RECT	 rect;

#ifdef __WIN32__
	GetWindowRect(hwnd, &rect);
	GetCursorPos(&point);
#else
	Window			 root_return;
	Window			 child_return;
	SMOOTHInt		 root_x_return;
	SMOOTHInt		 root_y_return;
	SMOOTHInt		 win_x_return;
	SMOOTHInt		 win_y_return;
	SMOOTHUnsignedInt	 mask_return;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);
	XQueryPointer(hwnd.display, hwnd.window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

	point.x = root_x_return;
	rect.left = window_attributes_return.x;
#endif

	if (wincl == SCREEN)		return point.x;
	else if (wincl == WINDOW)	return point.x-rect.left;
	else if (wincl == CLIENT)	return point.x-rect.left-2;

	return -1;
}

SMOOTHInt MouseY(HWND hwnd, SMOOTHInt wincl)
{
	POINT	 point;
	RECT	 rect;

#ifdef __WIN32__
	GetWindowRect(hwnd, &rect);
	GetCursorPos(&point);
#else
	Window			 root_return;
	Window			 child_return;
	SMOOTHInt		 root_x_return;
	SMOOTHInt		 root_y_return;
	SMOOTHInt		 win_x_return;
	SMOOTHInt		 win_y_return;
	SMOOTHUnsignedInt	 mask_return;
	XWindowAttributes	 window_attributes_return;

	if (hwnd == 0)
	{
		hwnd.display = default_display;
		hwnd.window = DefaultRootWindow(hwnd.display);
	}

	XGetWindowAttributes(hwnd.display, hwnd.window, &window_attributes_return);
	XQueryPointer(hwnd.display, hwnd.window, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

	point.y = root_y_return;
	rect.top = window_attributes_return.y;
#endif

	if (wincl == SCREEN)		return point.y;
	else if (wincl == WINDOW)	return point.y-rect.top;
	else if (wincl == CLIENT)	return point.y-rect.top-2;

	return -1;
}


#endif
