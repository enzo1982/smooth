 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DEFS_
#define _H_OBJSMOOTH_DEFS_

#include <lisa.h>

#ifdef __GNUC__
	#ifndef __int64
		#define __int64 long long
	#endif
#endif

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
	#elif defined __SMOOTH_PLUGIN_DLL__ && defined __SMOOTH_STATIC__
		#define SMOOTHAPI
		#define SMOOTHVAR extern

		#define SMOOTH_PLUGIN_API __declspec (dllexport)
		#define SMOOTH_PLUGIN_VAR extern __declspec (dllexport)

		#define __SMOOTH_DLL__
	#elif defined __SMOOTH_STATIC__
		#define SMOOTHAPI
		#define SMOOTHVAR extern

		#define SMOOTH_PLUGIN_API __declspec (dllimport)
		#define SMOOTH_PLUGIN_VAR __declspec (dllimport)

		#define __SMOOTH_DLL__
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

	#if !defined WM_MOUSEWHEEL
		#define WM_MOUSEWHEEL 522
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

#define RGB(r,g,b) CombineColor(r,g,b)
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#else
#define NIL	(0)
#endif

#include "types/bool.h"
#include "types/float.h"
#include "types/int.h"
#include "types/void.h"

namespace S = smooth;

namespace smooth
{
	const Int Success	= 1;
	const Int Error		= 0;
	const Int Break		= -1;

	const Bool True		= (Bool) -1;
	const Bool False	= (Bool) 0;

	Int	 Main();
	Void	 AttachDLL();
	Void	 DetachDLL();
};

#include "array.h"

#ifdef _MSC_VER
	#define Proc(x)		 (S::Void (S::Object::*)())(x)
	#define KillProc(x)	 (S::Bool (S::Object::*)())(x)
	#define MessageProc(x)	 (S::Void (S::Object::*)(S::Int,S::Int,S::Int))(x)
	#define ThreadProc(x)	 (S::Void (S::Object::*)(S::Thread *))(x)

	#define ProcType	 S::Void(S::Object::*)()
	#define KillProcType	 S::Bool(S::Object::*)()
	#define MessageProcType	 S::Void(S::Object::*)(S::Int,S::Int,S::Int)
	#define ThreadProcType	 S::Void(S::Object::*)(S::Thread *)

	#define ProcMember		 S::Void(S::Object::*proc)()
	#define PaintProcMember		 S::Void(S::Object::*paintProc)()
	#define PeekProcMember		 S::Void(S::Object::*peekProc)()
	#define KillProcMember		 S::Bool(S::Object::*killProc)()
	#define MessageProcMember	 S::Void(S::Object::*messageProc)(S::Int,S::Int,S::Int)
	#define ThreadProcMember	 S::Void(S::Object::*threadProc)(S::Thread *)

	#define ProcParam	 S::Void(S::Object::*newProc)()
	#define KillProcParam	 S::Bool(S::Object::*newProc)()
	#define MessageProcParam S::Void(S::Object::*newProc)(S::Int,S::Int,S::Int)
	#define ThreadProcParam	 S::Void(S::Object::*newProc)(S::Thread *)

	#define ProcCall(p,c)			 if(p!=NIL)(((S::Object *) c)->*p)()
	#define KillProcCall(p,c)		 (p==NIL)?(S::True):(((S::Object *) c)->*p)()
	#define MessageProcCall(p,c,m,w,l)	 if(p!=NIL)(((S::Object *) c)->*p)(m,w,l)
	#define ThreadProcCall(p,c,t)		 if(p!=NIL)(((S::Object *) c)->*p)(t)
#else
	#define Proc(x)		 (S::Void(*)())(x)
	#define KillProc(x)	 (S::Bool(*)())(x)
	#define MessageProc(x)	 (S::Void(*)(S::Int,S::Int,S::Int))(x)
	#define ThreadProc(x)	 (S::Void(*)(S::Thread *))(x)

	#define ProcType	 S::Void(*)(S::Void *)
	#define KillProcType	 S::Bool(*)(S::Void *)
	#define MessageProcType	 S::Void(*)(S::Void *,S::Int,S::Int,S::Int)
	#define ThreadProcType	 S::Void(*)(S::Void *,S::Thread *)

	#define ProcMember		 S::Void(*proc)(S::Void *)
	#define PaintProcMember		 S::Void(*paintProc)(S::Void *)
	#define PeekProcMember		 S::Void(*peekProc)(S::Void *)
	#define KillProcMember		 S::Bool(*killProc)(S::Void *)
	#define MessageProcMember	 S::Void(*messageProc)(S::Void *,S::Int,S::Int,S::Int)
	#define ThreadProcMember	 S::Void(*threadProc)(S::Void *,S::Thread *)

	#define ProcParam	 S::Void(*newProc)()
	#define KillProcParam	 S::Bool(*newProc)()
	#define MessageProcParam S::Void(*newProc)(S::Int,S::Int,S::Int)
	#define ThreadProcParam	 S::Void(*newProc)(S::Thread *)

	#define ProcCall(p,c)			 if(p!=NIL)p(c)
	#define KillProcCall(p,c)		 (p==NIL)?(S::True):p(c)
	#define MessageProcCall(p,c,m,w,l)	 if(p!=NIL)p(c, m, w, l)
	#define ThreadProcCall(p,c,t)		 if(p!=NIL)p(c, t)
#endif

#define NULLPROC NIL,NIL

const HBITMAP SI_DEFAULT		= (HBITMAP) -1;

const S::Int SM_MOUSEMOVE		= 1024;
const S::Int SM_LBUTTONDOWN		= 1025;
const S::Int SM_LBUTTONUP		= 1026;
const S::Int SM_RBUTTONDOWN		= 1027;
const S::Int SM_RBUTTONUP		= 1028;
const S::Int SM_LBUTTONDBLCLK		= 1029;
const S::Int SM_RBUTTONDBLCLK		= 1030;
const S::Int SM_WINDOWTITLECHANGED	= 1031;
const S::Int SM_LOSTCONN		= 1032;
const S::Int SM_MOUSELEAVE		= 1033;
const S::Int SM_SHOWTOOLTIP		= 1034;
const S::Int SM_MOUSEWHEEL		= 1035;

const S::Int SM_EXECUTEPEEK		= 1536;
const S::Int SM_CHECKCHECKBOXES		= 1537;
const S::Int SM_CHECKOPTIONBOXES	= 1538;
const S::Int SM_CHECKLISTBOXES		= 1539;
const S::Int SM_CHECKCOMBOBOXES		= 1540;
const S::Int SM_LOOSEFOCUS		= 1541;
const S::Int SM_TIMER			= 1542;
const S::Int SM_CHECKPOPUPS		= 1543;

const S::Int OR_UPPERLEFT		= 0;
const S::Int OR_UPPERRIGHT		= 1;
const S::Int OR_LOWERLEFT		= 2;
const S::Int OR_LOWERRIGHT		= 3;

const S::Int OR_HORZ			= 1;
const S::Int OR_VERT			= 2;

const S::Int OR_TOP			= 4;
const S::Int OR_BOTTOM			= 8;

const S::Int OR_LEFT			= 16;
const S::Int OR_RIGHT			= 32;

const S::Int OR_CENTER			= 64;
const S::Int OR_FREE			= 128;

#endif
