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
	namespace GUI
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
};

#include "widget.h"
#include "container.h"
#include "rect.h"
#include "loop.h"

namespace smooth
{
	namespace GUI
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

		class SMOOTHAPI Window : public Widget, public Container
		{
#ifdef __WIN32__
			friend		LRESULT CALLBACK	 WindowProc(HWND, UINT, WPARAM, LPARAM);
#endif

			friend class	S::SMOOTH;
			friend class	Menubar;
			friend class	Client;
			friend class	MDIClient;
			friend class	Divider;
			friend class	Titlebar;
			friend class	Layer;
			friend class	ToolWindow;
			friend		Int SMOOTHAPI S::Loop();
			protected:
				Int				 style;
				Int				 exstyle;
				Bool				 modal;
				Bool				 sysmodal;
				Bool				 apptopmost;
				Bool				 stay;
				Bool				 maximized;

				Bool				 created;
				Bool				 destroyed;

#ifdef __WIN32__
				HBITMAP				 icon;
				HICON				 sysicon;
#endif

				Rect				 offset;
				Rect				 updateRect;

				Size				 minSize;

				String				 className;

				PopupMenu			*popupMenu;

#ifdef __WIN32__
				HDC				 windowDC;
#endif

				Layer				*mainLayer;

#ifdef __WIN32__
				HWND				 Create();
#endif

				Void				 CalculateOffsets();

				Bool				 DummyExitProc();
			public:
				static Int			 nOfActiveWindows;
				Int				 value;

#ifdef __WIN32__
				HWND				 hwnd;
#endif

				Bool				 cursorset;
				Bool				 initshow;

								 Window(String title = NIL);
								~Window();

#ifdef __WIN32__
				Void				 SetPositionFlag(HWND);
				Int				 SetIcon(HBITMAP);
				Int				 SetApplicationIcon(HICON);
#endif

				Int				 SetApplicationIcon(Int);
				Int				 SetMetrics(Point, Size);
				Void				 SetStyle(Int);
				Void				 SetExStyle(Int);
				Int				 SetText(String);

				Int				 SetStatusText(String);
				String				 GetStatusText();

				Rect				 GetUpdateRect();
				Int				 SetUpdateRect(Rect);

				Int				 SetMinimumSize(Size);

				Int				 Show();
				Int				 Hide();

				Int				 Close();

				Int				 Stay();

				Bool				 IsMaximized();

				Int				 Paint(Int);
				Int				 Process(Int, Int, Int);

				Int				 MouseX();
				Int				 MouseY();

				Bool				 IsMouseOn(Rect);

				Int				 RegisterObject(Object *);
				Int				 UnregisterObject(Object *);

				static Window			*GetWindow(HWND);
			signals:
				Signal0<Void>			 onPaint;
				Signal0<Void>			 onPeek;
				Signal3<Void, Int, Int, Int>	 onEvent;
			callbacks:
				Callback0<Bool>			 doQuit;
		};
	};

	SMOOTHVAR Int OBJ_WINDOW;
};

#endif
