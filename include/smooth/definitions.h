 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_DEFS_
#define _H_OBJSMOOTH_DEFS_

#include "types/bool.h"
#include "types/byte.h"
#include "types/float.h"
#include "types/int.h"
#include "types/short.h"
#include "types/void.h"

#include "system.h"

#include "point.h"
#include "size.h"
#include "rect.h"
#include "line.h"

#include "string.h"
#include "array.h"

#ifdef _MSC_VER
	#define SMOOTHProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SMOOTHPaintProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SMOOTHPeekProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SMOOTHKillProc(x,p,y)	 (SMOOTHBool (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SMOOTHMessageProc(x,p,y) (SMOOTHVoid (SMOOTHObject::*)(SMOOTHInt,SMOOTHInt,SMOOTHInt))p->y,(SMOOTHVoid *)p
	#define SMOOTHThreadProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)(SMOOTHThread *))p->y,(SMOOTHVoid *)p

	#define SProc(x,p,y)		 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SPaintProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SPeekProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SKillProc(x,p,y)	 (SMOOTHBool (SMOOTHObject::*)())p->y,(SMOOTHVoid *)p
	#define SMessageProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)(SMOOTHInt,SMOOTHInt,SMOOTHInt))p->y,(SMOOTHVoid *)p
	#define SThreadProc(x,p,y)	 (SMOOTHVoid (SMOOTHObject::*)(SMOOTHThread *))p->y,(SMOOTHVoid *)p

	#define SMOOTHProcType		 SMOOTHVoid(SMOOTHObject::*)()
	#define SMOOTHPaintProcType	 SMOOTHVoid(SMOOTHObject::*)()
	#define SMOOTHPeekProcType	 SMOOTHVoid(SMOOTHObject::*)()
	#define SMOOTHKillProcType	 SMOOTHBool(SMOOTHObject::*)()
	#define SMOOTHMessageProcType	 SMOOTHVoid(SMOOTHObject::*)(SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcType	 SMOOTHVoid(SMOOTHObject::*)(SMOOTHThread *)

	#define SMOOTHProcMember	 SMOOTHVoid(SMOOTHObject::*proc)()
	#define SMOOTHPaintProcMember	 SMOOTHVoid(SMOOTHObject::*paintProc)()
	#define SMOOTHPeekProcMember	 SMOOTHVoid(SMOOTHObject::*peekProc)()
	#define SMOOTHKillProcMember	 SMOOTHBool(SMOOTHObject::*killProc)()
	#define SMOOTHMessageProcMember	 SMOOTHVoid(SMOOTHObject::*messageProc)(SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcMember	 SMOOTHVoid(SMOOTHObject::*threadProc)(SMOOTHThread *)

	#define SMOOTHProcParam		 SMOOTHVoid(SMOOTHObject::*newProc)()
	#define SMOOTHPaintProcParam	 SMOOTHVoid(SMOOTHObject::*newProc)()
	#define SMOOTHPeekProcParam	 SMOOTHVoid(SMOOTHObject::*newProc)()
	#define SMOOTHKillProcParam	 SMOOTHBool(SMOOTHObject::*newProc)()
	#define SMOOTHMessageProcParam	 SMOOTHVoid(SMOOTHObject::*newProc)(SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcParam	 SMOOTHVoid(SMOOTHObject::*newProc)(SMOOTHThread *)

	#define SMOOTHProcCall(p,c)		 if(p!=NIL)(((SMOOTHObject *) c)->*p)()
	#define SMOOTHPaintProcCall(p,c)	 if(p!=NIL)(((SMOOTHObject *) c)->*p)()
	#define SMOOTHPeekProcCall(p,c)		 if(p!=NIL)(((SMOOTHObject *) c)->*p)()
	#define SMOOTHKillProcCall(p,c)		 (p==NIL)?(SMOOTH::True):(((SMOOTHObject *) c)->*p)()
	#define SMOOTHMessageProcCall(p,c,m,w,l) if(p!=NIL)(((SMOOTHObject *) c)->*p)(m,w,l)
	#define SMOOTHThreadProcCall(p,c,t)	 if(p!=NIL)(((SMOOTHObject *) c)->*p)(t)
#else
	#define SMOOTHProc(x,p,y)	 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMOOTHPaintProc(x,p,y)	 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMOOTHPeekProc(x,p,y)	 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMOOTHKillProc(x,p,y)	 (SMOOTHBool(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMOOTHMessageProc(x,p,y) (SMOOTHVoid(*)(SMOOTHInt,SMOOTHInt,SMOOTHInt))(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMOOTHThreadProc(x,p,y)	 (SMOOTHVoid(*)(SMOOTHThread *))(p->*(&x::y)),(SMOOTHVoid *)p

	#define SProc(x,p,y)		 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SPaintProc(x,p,y)	 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SPeekProc(x,p,y)	 (SMOOTHVoid(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SKillProc(x,p,y)	 (SMOOTHBool(*)())(p->*(&x::y)),(SMOOTHVoid *)p
	#define SMessageProc(x,p,y)	 (SMOOTHVoid(*)(SMOOTHInt,SMOOTHInt,SMOOTHInt))(p->*(&x::y)),(SMOOTHVoid *)p
	#define SThreadProc(x,p,y)	 (SMOOTHVoid(*)(SMOOTHThread *))(p->*(&x::y)),(SMOOTHVoid *)p

	#define SMOOTHProcType		 SMOOTHVoid(*)(SMOOTHVoid *)
	#define SMOOTHPaintProcType	 SMOOTHVoid(*)(SMOOTHVoid *)
	#define SMOOTHPeekProcType	 SMOOTHVoid(*)(SMOOTHVoid *)
	#define SMOOTHKillProcType	 SMOOTHBool(*)(SMOOTHVoid *)
	#define SMOOTHMessageProcType	 SMOOTHVoid(*)(SMOOTHVoid *,SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcType	 SMOOTHVoid(*)(SMOOTHVoid *,SMOOTHThread *)

	#define SMOOTHProcMember	 SMOOTHVoid(*proc)(SMOOTHVoid *)
	#define SMOOTHPaintProcMember	 SMOOTHVoid(*paintProc)(SMOOTHVoid *)
	#define SMOOTHPeekProcMember	 SMOOTHVoid(*peekProc)(SMOOTHVoid *)
	#define SMOOTHKillProcMember	 SMOOTHBool(*killProc)(SMOOTHVoid *)
	#define SMOOTHMessageProcMember	 SMOOTHVoid(*messageProc)(SMOOTHVoid *,SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcMember	 SMOOTHVoid(*threadProc)(SMOOTHVoid *,SMOOTHThread *)

	#define SMOOTHProcParam		 SMOOTHVoid(*newProc)()
	#define SMOOTHPaintProcParam	 SMOOTHVoid(*newProc)()
	#define SMOOTHPeekProcParam	 SMOOTHVoid(*newProc)()
	#define SMOOTHKillProcParam	 SMOOTHBool(*newProc)()
	#define SMOOTHMessageProcParam	 SMOOTHVoid(*newProc)(SMOOTHInt,SMOOTHInt,SMOOTHInt)
	#define SMOOTHThreadProcParam	 SMOOTHVoid(*newProc)(SMOOTHThread *)

	#define SMOOTHProcCall(p,c)		 if(p!=NIL)p(c)
	#define SMOOTHPaintProcCall(p,c)	 if(p!=NIL)p(c)
	#define SMOOTHPeekProcCall(p,c)		 if(p!=NIL)p(c)
	#define SMOOTHKillProcCall(p,c)		 (p==NIL)?(SMOOTH::True):p(c)
	#define SMOOTHMessageProcCall(p,c,m,w,l) if(p!=NIL)p(c, m, w, l)
	#define SMOOTHThreadProcCall(p,c,t)	 if(p!=NIL)p(c, t)
#endif

#define NULLPROC NIL,NIL

const HBITMAP SI_DEFAULT		= (HBITMAP) -1;

const SMOOTHInt SM_MOUSEMOVE		= 1024;
const SMOOTHInt SM_LBUTTONDOWN		= 1025;
const SMOOTHInt SM_LBUTTONUP		= 1026;
const SMOOTHInt SM_RBUTTONDOWN		= 1027;
const SMOOTHInt SM_RBUTTONUP		= 1028;
const SMOOTHInt SM_LBUTTONDBLCLK	= 1029;
const SMOOTHInt SM_RBUTTONDBLCLK	= 1030;
const SMOOTHInt SM_WINDOWTITLECHANGED	= 1031;
const SMOOTHInt SM_LOSTCONN		= 1032;
const SMOOTHInt SM_MOUSELEAVE		= 1033;
const SMOOTHInt SM_SHOWTOOLTIP		= 1034;

const SMOOTHInt SM_EXECUTEPEEK		= 1536;
const SMOOTHInt SM_CHECKCHECKBOXES	= 1537;
const SMOOTHInt SM_CHECKOPTIONBOXES	= 1538;
const SMOOTHInt SM_CHECKLISTBOXES	= 1539;
const SMOOTHInt SM_CHECKCOMBOBOXES	= 1540;
const SMOOTHInt SM_LOOSEFOCUS		= 1541;
const SMOOTHInt SM_TIMER		= 1542;
const SMOOTHInt SM_CHECKPOPUPS		= 1543;

const SMOOTHInt OR_UPPERLEFT		= 0;
const SMOOTHInt OR_UPPERRIGHT		= 1;
const SMOOTHInt OR_LOWERLEFT		= 2;
const SMOOTHInt OR_LOWERRIGHT		= 3;

const SMOOTHInt OR_HORZ			= 1;
const SMOOTHInt OR_VERT			= 2;

const SMOOTHInt OR_TOP			= 4;
const SMOOTHInt OR_BOTTOM		= 8;

const SMOOTHInt OR_LEFT			= 16;
const SMOOTHInt OR_RIGHT		= 32;

const SMOOTHInt OR_CENTER		= 64;
const SMOOTHInt OR_FREE			= 128;

#endif
