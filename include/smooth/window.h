 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOW_
#define _H_OBJSMOOTH_WINDOW_

#define SWindow SMOOTHWindow

class SMOOTHWindow;
class SMOOTHToolWindow;
class SMOOTHPopupMenu;
class SMOOTHMenubar;
class SMOOTHTitlebar;
class SMOOTHClient;
class SMOOTHMDIClient;
class SMOOTHLayer;
class SMOOTHDivisionbar;

#include "object.h"
#include "container.h"

const SMOOTHInt SS_MODAL	= 1;
const SMOOTHInt SS_SYSMODAL	= 2;
const SMOOTHInt SS_APPTOPMOST	= 3;
const SMOOTHInt SS_NORESIZE	= 4;

const SMOOTHInt WO_SEPARATOR	= 1;
const SMOOTHInt WO_NOSEPARATOR	= 2;

#ifdef __SMOOTH_DLL__
#ifdef __WIN32__
LRESULT CALLBACK SMOOTHWindowProc(HWND, UINT, WPARAM, LPARAM);
#endif
#endif

class SMOOTHAPI SMOOTHWindow : public SMOOTHObject, public SMOOTHContainer
{
#ifdef __WIN32__
	friend		LRESULT CALLBACK	 SMOOTHWindowProc(HWND, UINT, WPARAM, LPARAM);
#endif

	friend class	SMOOTH;
	friend class	SMOOTHMenubar;
	friend class	SMOOTHClient;
	friend class	SMOOTHMDIClient;
	friend class	SMOOTHDivisionbar;
	friend class	SMOOTHTitlebar;
	friend class	SMOOTHLayer;
	friend class	SMOOTHToolWindow;
	protected:
		SMOOTHInt			 style;
		SMOOTHInt			 exstyle;
		SMOOTHBool			 modal;
		SMOOTHBool			 sysmodal;
		SMOOTHBool			 apptopmost;
		SMOOTHBool			 stay;
		SMOOTHBool			 maximized;

		SMOOTHBool			 created;
		SMOOTHBool			 destroyed;

		HBITMAP				 icon;
		HICON				 sysicon;

		SMOOTHRect			 offset;
		SMOOTHRect			 updateRect;

		SMOOTHSize			 minSize;

		SMOOTHString			 className;

		SMOOTHKillProcMember;
		SMOOTHVoid			*killProcParam;
		SMOOTHPaintProcMember;
		SMOOTHVoid			*paintProcParam;
		SMOOTHPeekProcMember;
		SMOOTHVoid			*peekProcParam;
		SMOOTHMessageProcMember;
		SMOOTHVoid			*messageProcParam;

		SMOOTHPopupMenu			*popupMenu;

		HDC				 windowDC;

		HWND				 Create();
		SMOOTHVoid			 CalculateOffsets();
	public:
		static SMOOTHInt		 nOfActiveWindows;
		SMOOTHInt			 value;

		HWND				 hwnd;
		SMOOTHBool			 cursorset;
		SMOOTHBool			 initshow;

						 SMOOTHWindow(SMOOTHString title = NIL);
						~SMOOTHWindow();

		SMOOTHInt			 SetMetrics(SMOOTHPoint, SMOOTHSize);
		SMOOTHVoid			 SetPositionFlag(HWND);
		SMOOTHVoid			 SetStyle(SMOOTHInt);
		SMOOTHVoid			 SetExStyle(SMOOTHInt);
		SMOOTHInt			 SetIcon(HBITMAP);
		SMOOTHInt			 SetApplicationIcon(HICON);
		SMOOTHInt			 SetApplicationIcon(SMOOTHInt);
		SMOOTHInt			 SetText(SMOOTHString);

		SMOOTHInt			 SetStatusText(SMOOTHString);
		SMOOTHString			 GetStatusText();

		SMOOTHVoid			 SetPaintProc(SMOOTHPaintProcParam, SMOOTHVoid *);
		SMOOTHVoid			 SetKillProc(SMOOTHKillProcParam, SMOOTHVoid *);
		SMOOTHVoid			 SetPeekProc(SMOOTHPeekProcParam, SMOOTHVoid *);
		SMOOTHVoid			 SetMessageProc(SMOOTHMessageProcParam, SMOOTHVoid *);

		SMOOTHRect			 GetUpdateRect();
		SMOOTHInt			 SetUpdateRect(SMOOTHRect);

		SMOOTHInt			 SetMinimumSize(SMOOTHSize);

		SMOOTHInt			 Show();
		SMOOTHInt			 Hide();
		SMOOTHInt			 Stay();

		SMOOTHBool			 IsMaximized();

		SMOOTHInt			 Paint(SMOOTHInt);
		SMOOTHInt			 Process(SMOOTHInt, SMOOTHInt, SMOOTHInt);

		SMOOTHInt			 RegisterObject(SMOOTHObject *);
		SMOOTHInt			 UnregisterObject(SMOOTHObject *);
};

SMOOTHVAR SMOOTHInt OBJ_WINDOW;

#endif
