 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOW_
#define _H_OBJSMOOTH_WINDOW_

namespace smooth
{
	class Window;
	class ToolWindow;
	class PopupMenu;
	class Menubar;
	class Titlebar;
	class Client;
	class MDIClient;
	class Layer;
	class Divider;
};

#include "object.h"
#include "container.h"
#include "rect.h"

namespace smooth
{
	const Int SS_MODAL		= 1;
	const Int SS_SYSMODAL		= 2;
	const Int SS_APPTOPMOST		= 3;
	const Int SS_NORESIZE		= 4;

	const Int WO_SEPARATOR		= 1;
	const Int WO_NOSEPARATOR	= 2;

#ifdef __SMOOTH_DLL__
#ifdef __WIN32__
	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
#endif
#endif

	class SMOOTHAPI Window : public Object, public Container
	{
#ifdef __WIN32__
		friend		LRESULT CALLBACK	 WindowProc(HWND, UINT, WPARAM, LPARAM);
#endif

		friend class	SMOOTH;
		friend class	Menubar;
		friend class	Client;
		friend class	MDIClient;
		friend class	Divider;
		friend class	Titlebar;
		friend class	Layer;
		friend class	ToolWindow;
		friend		Int SMOOTHAPI Loop();
		protected:
			Int		 style;
			Int		 exstyle;
			Bool		 modal;
			Bool		 sysmodal;
			Bool		 apptopmost;
			Bool		 stay;
			Bool		 maximized;

			Bool		 created;
			Bool		 destroyed;

			HBITMAP		 icon;
			HICON		 sysicon;

			Rect		 offset;
			Rect		 updateRect;

			Size		 minSize;

			String		 className;

			KillProcMember;
			Void		*killProcParam;
			PaintProcMember;
			Void		*paintProcParam;
			PeekProcMember;
			Void		*peekProcParam;
			MessageProcMember;
			Void		*messageProcParam;

			PopupMenu	*popupMenu;

			HDC		 windowDC;

			Layer		*mainLayer;

			HWND		 Create();
			Void		 CalculateOffsets();
		public:
			static Int	 nOfActiveWindows;
			Int		 value;

			HWND		 hwnd;
			Bool		 cursorset;
			Bool		 initshow;

					 Window(String title = NIL);
					~Window();

			Int		 SetMetrics(Point, Size);
			Void		 SetPositionFlag(HWND);
			Void		 SetStyle(Int);
			Void		 SetExStyle(Int);
			Int		 SetIcon(HBITMAP);
			Int		 SetApplicationIcon(HICON);
			Int		 SetApplicationIcon(Int);
			Int		 SetText(String);

			Int		 SetStatusText(String);
			String		 GetStatusText();

			Void		 SetPaintProc(ProcParam, Void *);
			Void		 SetKillProc(KillProcParam, Void *);
			Void		 SetPeekProc(ProcParam, Void *);
			Void		 SetMessageProc(MessageProcParam, Void *);

			Rect		 GetUpdateRect();
			Int		 SetUpdateRect(Rect);

			Int		 SetMinimumSize(Size);

			Int		 Show();
			Int		 Hide();

			Int		 Close();

			Int		 Stay();

			Bool		 IsMaximized();

			Int		 Paint(Int);
			Int		 Process(Int, Int, Int);

			Int		 RegisterObject(Object *);
			Int		 UnregisterObject(Object *);
	};

	SMOOTHVAR Int OBJ_WINDOW;
};

#endif
