 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_WINDOWGDI_
#define _H_OBJSMOOTH_WINDOWGDI_

namespace smooth
{
	namespace GUI
	{
		class WindowGDI;
	};
};

#include "../windowbackend.h"
#include "../../primitives/rect.h"

namespace smooth
{
	namespace GUI
	{
		const Int	 WINDOW_GDI	= 1;

		class WindowGDI : public WindowBackend
		{
			private:
				static LRESULT CALLBACK		 WindowProc(HWND, UINT, WPARAM, LPARAM);

				static Array<WindowGDI *>	 windowBackends;

				static WindowGDI		*GetWindowBackend(HWND);

				Int				 ProcessSystemMessages(Int, Int, Int);
			protected:
				Int				 id;

				HWND				 hwnd;
				HDC				 windowDC;

				Void				*wndclass;
				String				 className;

				HICON				 sysIcon;

				Int				 origWndStyle;
				Rect				 nonMaxRect;

				Size				 minSize;
				Size				 maxSize;

				Bool				 maximized;
			public:
								 WindowGDI(Void * = NIL);
								~WindowGDI();

				Void				*GetSystemWindow();

				Int				 Open(String, Point, Size, Int);
				Int				 Close();

				Int				 SetTitle(String);
				Int				 SetIcon(Bitmap &);

				Int				 SetMinimumSize(Size);
				Int				 SetMaximumSize(Size);

				Int				 Show();
				Int				 Hide();

				Rect				 GetRestoredWindowRect();

				Int				 SetMetrics(const Point &, const Size &);

				Int				 Minimize();

				Int				 Maximize();
				Int				 Restore();
		};
	};
};

#endif
