 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TOOLKIT_
#define _H_TOOLKIT_

#include "system.h"
#include "point.h"
#include "rect.h"
#include "size.h"
#include "window.h"

int	 GetTextSizeX	(HDC, SMOOTHString, int, SMOOTHString, int, int, bool);
int	 GetLineSizeX	(HDC, SMOOTHString, int, SMOOTHString, int, int, bool);
int	 GetLineSizeY	(HDC, SMOOTHString, SMOOTHString, int, int);

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
SMOOTHRect	 GetBitmapMetrics	(HBITMAP);
void		 Line			(HDC, SMOOTHPoint, SMOOTHPoint, long, int, int);
void		 Frame			(HDC, SMOOTHRect, int);
void		 Gradient		(HDC, SMOOTHRect, long, long, int);
void		 Box			(HDC, SMOOTHRect, int, int);
void		 HBar			(HDC, SMOOTHPoint, SMOOTHPoint);
void		 VBar			(HDC, SMOOTHPoint, SMOOTHPoint);
void		 PaintPixel		(HDC, SMOOTHPoint, int);
int		 GetPixel		(HDC, SMOOTHPoint);
void		 PaintBitmap		(HDC, SMOOTHRect, HBITMAP);
void		 SetText		(HDC, SMOOTHString, SMOOTHRect, SMOOTHString, int, int, int);
void		 SetShadowedText	(HDC, SMOOTHString, SMOOTHRect, SMOOTHString, int, int, int);
bool		 IsMouseOn		(HWND, SMOOTHRect, int);
int		 GetTextSizeX		(HDC, SMOOTHString, SMOOTHString, int, int);
int		 GetTextSizeX		(HDC, SMOOTHString, int, SMOOTHString, int, int);
int		 GetTextSizeXNoExtend	(HDC, SMOOTHString, SMOOTHString, int, int);
int		 GetTextSizeXNoExtend	(HDC, SMOOTHString, int, SMOOTHString, int, int);
int		 GetTextSizeY		(HDC, SMOOTHString, SMOOTHString, int, int);
int		 MouseX			(HWND, int);
int		 MouseY			(HWND, int);
bool		 DoRectsOverlap		(SMOOTHRect, SMOOTHRect);
SMOOTHRect	 OverlapRect		(SMOOTHRect, SMOOTHRect);
HDC		 GetContext		(SMOOTHWindow *);
void		 FreeContext		(SMOOTHWindow *, HDC);
HDC		 CreateCompatibleContext(HDC, SMOOTHSize);
void		 FreeCompatibleContext	(HDC);
HBITMAP		 BlitToBitmap		(HDC, SMOOTHRect);
bool		 BlitToBitmap		(SMOOTHRect, HDC, SMOOTHRect, HBITMAP);
bool		 BlitFromBitmap		(SMOOTHRect, HBITMAP, SMOOTHRect, HDC);
bool		 DestroyBitmap		(HBITMAP);
HWND		 CreateSimpleWindow	(SMOOTHRect, SMOOTHString, SMOOTHString, HICON, int, int);
int		 ConvertString		(const char *, int, const char *, char *, int, const char *);

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

#endif
