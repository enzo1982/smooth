 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_SYSTEM_
#define _H_SYSTEM_

#include <lisa.h>

#if defined __WIN32__
	#if defined __SMOOTH_DLL__ && !defined WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif

	#include <windows.h>
	#include <wtypes.h>
	#include <tchar.h>
	#include <wchar.h>

	#if defined __SMOOTH_DLL__
		#define SMOOTHAPI __declspec (dllexport)
		#define SMOOTHVAR extern __declspec (dllexport)
	#elif defined __SMOOTH_PLUGIN_DLL__
		#define SMOOTHAPI __declspec (dllimport)
		#define SMOOTHVAR __declspec (dllimport)

		#define SMOOTH_PLUGIN_API __declspec (dllexport)
		#define SMOOTH_PLUGIN_VAR extern __declspec (dllexport)
	#else
		#define SMOOTHAPI __declspec (dllimport)
		#define SMOOTHVAR __declspec (dllimport)

		#define SMOOTH_PLUGIN_API __declspec (dllimport)
		#define SMOOTH_PLUGIN_VAR __declspec (dllimport)
	#endif
#elif defined __QNX__
	#include <Ph.h>
	#include <Pt.h>
	#include <wchar.h>

	#define SMOOTHAPI
	#define SMOOTHVAR extern

	#define SMOOTH_PLUGIN_API
	#define SMOOTH_PLUGIN_VAR extern
#else
	#include <X11/Xlib.h>
	#include <wchar.h>

	#define SMOOTHAPI
	#define SMOOTHVAR extern

	#define SMOOTH_PLUGIN_API
	#define SMOOTH_PLUGIN_VAR extern

	#undef Success
	#undef True
	#undef False

	#define _TEXT(x) L##x
#endif

#ifndef __WIN32__
#ifndef __WINDOWS_TYPES__
#define __WINDOWS_TYPES__

#define NIL	(0)

#define PS_SOLID	0
#define PS_DASH		1
#define PS_DOT		2
#define PS_DASHDOT	3
#define PS_DASHDOTDOT	4
#define PS_NULL		5

#ifdef __QNX__
    #define HBITMAP	PhImage_t *
    #define HICON	PhImage_t *
#else
    #define HBITMAP	Pixmap
    #define HICON	Pixmap

#endif

class HWND
{
	public:
					 HWND();
					 HWND(int);
#ifdef __QNX__
		PtWindowWidget_t	*window;
#else
		Display			*display;
		Window			 window;
#endif
		const HWND &operator	 =(int);
		bool operator		 ==(int);
		bool operator		 !=(int);
		bool operator		 ==(HWND);
		bool operator		 !=(HWND);
};

class HDC
{
	public:
					 HDC();
					 HDC(int);
#ifdef __QNX__
		PhDrawContext_t		 context;
#else
		Display			*display;
		Drawable		 drawable;
		GC			 context;
#endif
		const HDC &operator	 =(int);
		bool operator		 ==(int);
		bool operator		 !=(int);
};

#define RGB(r,g,b) CombineColor(r,g,b)
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#else
#define NIL	(0)
#endif

#endif
