 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
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
		class WindowBase;
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

#include "../widget.h"
#include "../container.h"
#include "../rect.h"
#include "../loop.h"
#include "../menu.h"
#include "../timer.h"
#include "bitmap.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 WF_MODAL		= 1;
		const Int	 WF_SYSTEMMODAL		= 2;
		const Int	 WF_TOPMOST		= 4;
		const Int	 WF_APPTOPMOST		= 8;
		const Int	 WF_NORESIZE		= 16;
		const Int	 WF_NOTASKBUTTON	= 32;
		const Int	 WF_DELAYPAINT		= 64;

		const Int	 WO_SEPARATOR	= 1;
		const Int	 WO_NOSEPARATOR	= 2;

		class SMOOTHAPI WindowBase : public Widget, public Container
		{
			protected:
				Int				 style;
				Int				 exstyle;
				Bool				 stay;
				Bool				 maximized;
				Rect				 nonmaxrect;

				Bool				 created;
				Bool				 destroyed;

				Bitmap				 icon;

				Rect				 innerOffset;
				Rect				 updateRect;
				Rect				 timedUpdateRect;

				Size				 minSize;
				Size				 maxSize;

				String				 className;

				PopupMenu			*trackMenu;

				Layer				*mainLayer;
				Timer				*paintTimer;

				virtual Bool			 Create();

				Void				 CalculateOffsets();

				Bool				 DummyExitProc();

				Void				 PopupProc();
			public:
				static const Int		 classID;

				static Int			 nOfActiveWindows;
				Int				 value;

				Bool				 initshow;

								 WindowBase(String title = NIL);
								~WindowBase();

				virtual Int			 SetIcon(const Bitmap &);
				Bitmap				&GetIcon();

				Int				 SetApplicationIcon(char *);
				Int				 SetApplicationIcon(wchar_t *);

				virtual Int			 SetMetrics(Point, Size);
				Void				 SetStyle(Int);
				Void				 SetExStyle(Int);
				virtual Int			 SetText(String);

				Layer				*GetMainLayer();

				Int				 SetStatusText(String);
				String				 GetStatusText();

				Rect				 GetWindowRect();
				Rect				 GetClientRect();

				Rect				 GetUpdateRect();
				Int				 SetUpdateRect(Rect);

				Int				 SetMinimumSize(Size);
				Int				 SetMaximumSize(Size);

				virtual Int			 Show();
				virtual Int			 Hide();

				virtual Int			 Maximize();
				virtual Int			 Restore();
				Bool				 IsMaximized();

				virtual Int			 Stay();
				virtual Int			 Close();

				Bool				 IsInUse();

				virtual Int			 Paint(Int);
				virtual Int			 Process(Int, Int, Int);

				Int				 MouseX();
				Int				 MouseY();

				virtual Bool			 IsMouseOn(Rect);

				virtual Int			 RegisterObject(Object *);
				virtual Int			 UnregisterObject(Object *);
			signals:
				Signal0<Void>			 onCreate;

				Signal0<Void>			 onPaint;
				Signal0<Void>			 onResize;
				Signal0<Void>			 onPeek;
				Signal3<Void, Int, Int, Int>	 onEvent;
			callbacks:
				Callback2<Menu *, Int, Int>	 getTrackMenu;
				Callback0<Bool>			 doQuit;
			slots:
				Void				 PaintTimer();
		};
	};
};

#ifdef __WIN32__
	#include "gdi/windowgdi.h"
	#define Window WindowGDI
#else
	#define Window WindowBase
#endif

#endif
