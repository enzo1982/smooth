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

int	 GetTextSizeX	(HDC, S::String, int, S::String, int, int, bool);
int	 GetLineSizeX	(HDC, S::String, int, S::String, int, int, bool);
int	 GetLineSizeY	(HDC, S::String, S::String, int, int);

#ifdef __SMOOTH_DLL__
inline int	 GetRed			(unsigned long);
inline int	 GetGreen		(unsigned long);
inline int	 GetBlue		(unsigned long);
inline int	 GetAlpha		(unsigned long);
long		 CombineColor		(int, int, int);
long		 CombineColor		(int, int, int, int);
int		 WinX			(HWND, int);
int		 WinY			(HWND, int);
int		 GetBitmapSizeX		(HBITMAP);
int		 GetBitmapSizeY		(HBITMAP);
S::Rect		 GetBitmapMetrics	(HBITMAP);
void		 Line			(HDC, S::Point, S::Point, long, int, int);
void		 Frame			(HDC, S::Rect, int);
void		 Gradient		(HDC, S::Rect, long, long, int);
void		 Box			(HDC, S::Rect, int, int);
void		 HBar			(HDC, S::Point, S::Point);
void		 VBar			(HDC, S::Point, S::Point);
void		 PaintPixel		(HDC, S::Point, int);
int		 GetPixel		(HDC, S::Point);
void		 PaintBitmap		(HDC, S::Rect, HBITMAP);
void		 SetText		(HDC, S::String, S::Rect, S::String, int, int, int);
void		 SetShadowedText	(HDC, S::String, S::Rect, S::String, int, int, int);
bool		 IsMouseOn		(HWND, S::Rect, int);
int		 GetTextSizeX		(HDC, S::String, S::String, int, int);
int		 GetTextSizeX		(HDC, S::String, int, S::String, int, int);
int		 GetTextSizeXNoExtend	(HDC, S::String, S::String, int, int);
int		 GetTextSizeXNoExtend	(HDC, S::String, int, S::String, int, int);
int		 GetTextSizeY		(HDC, S::String, S::String, int, int);
int		 MouseX			(HWND, int);
int		 MouseY			(HWND, int);
bool		 DoRectsOverlap		(S::Rect, S::Rect);
S::Rect		 OverlapRect		(S::Rect, S::Rect);
HDC		 GetContext		(S::Window *);
void		 FreeContext		(S::Window *, HDC);
HDC		 CreateCompatibleContext(HDC, S::Size);
void		 FreeCompatibleContext	(HDC);
HBITMAP		 BlitToBitmap		(HDC, S::Rect);
bool		 BlitToBitmap		(S::Rect, HDC, S::Rect, HBITMAP);
bool		 BlitFromBitmap		(S::Rect, HBITMAP, S::Rect, HDC);
bool		 DestroyBitmap		(HBITMAP);
HWND		 CreateSimpleWindow	(S::Rect, S::String, S::String, HICON, int, int);

const int	 FRAME_UP	= 0;
const int	 FRAME_DOWN	= 1;
const int	 GRADIENT_LR	= 0;
const int	 GRADIENT_TB	= 1;
const int	 WINDOW		= 0;
const int	 CLIENT		= 1;
const int	 SCREEN		= 2;
const int	 FILLED		= 0;
const int	 OUTLINED	= 1;
const int	 OUTLINEDOTS	= 2;
#endif

inline int GetRed(unsigned long color)
{ return color & 255; }

inline int GetGreen(unsigned long color)
{ return (color >> 8) & 255; }

inline int GetBlue(unsigned long color)
{ return (color >> 16) & 255; }

inline int GetAlpha(unsigned long color)
{ return (color >> 24) & 255; }

inline long CombineColor(int a, int b, int c)
{ return a + b*256 + c*65536; }

inline long CombineColor(int a, int b, int c, int d)
{ return a + b*256 + c*65536 + d*16777216; }

#endif
