 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TOOLKIT_
#define _H_OBJSMOOTH_TOOLKIT_

#include "system.h"
#include "point.h"
#include "rect.h"
#include "size.h"
#include "window.h"

int	 GetLineSizeX	(S::String, int, S::String, int, int);
int	 GetLineSizeY	(S::String, S::String, int, int);

#ifdef __SMOOTH_DLL__
int		 GetBitmapSizeX		(HBITMAP);
int		 GetBitmapSizeY		(HBITMAP);
S::Rect		 GetBitmapMetrics	(HBITMAP);
int		 GetTextSizeX		(S::String, int, S::String, int, int);
int		 GetTextSizeX		(S::String, S::String, int, int);
int		 GetTextSizeY		(S::String, S::String, int, int);
bool		 DoRectsOverlap		(S::Rect, S::Rect);
S::Rect		 OverlapRect		(S::Rect, S::Rect);
HDC		 GetContext		(S::GUI::Window *);
void		 FreeContext		(S::GUI::Window *, HDC);
HDC		 CreateCompatibleContext(HDC, S::Size);
void		 FreeCompatibleContext	(HDC);
HBITMAP		 BlitToBitmap		(HDC, S::Rect);
bool		 BlitToBitmap		(S::Rect, HDC, S::Rect, HBITMAP);
bool		 BlitFromBitmap		(S::Rect, HBITMAP, S::Rect, HDC);
HWND		 CreateSimpleWindow	(S::Rect, S::String, S::String, HICON, int, int);
#endif

#endif
