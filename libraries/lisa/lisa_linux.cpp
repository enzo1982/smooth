 /* LiSA - Library for System Adaptation
  * Copyright (C) 2002-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#include <lisa.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <X11/Xlib.h>

Display	*default_display = NULL;

void LiSAInit()
{
	default_display = XOpenDisplay(NULL);
}

void LiSADeinit()
{
	XCloseDisplay(default_display);
}

int LiSASetMouseCursor(LiSAWindow *window, int mouseCursor)
{
	if (mouseCursor == LiSA_MOUSE_ARROW)	return 0;
	else					return 1;
}

int LiSAOpenURLA(char *url)
{
	return 1;
}

int LiSAOpenURLW(wchar_t *url)
{
	return 1;
}

int LiSAGetDisplaySizeX()
{
	return WidthOfScreen(DefaultScreenOfDisplay(default_display));
}

int LiSAGetDisplaySizeY()
{
	return HeightOfScreen(DefaultScreenOfDisplay(default_display));
}
