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

#include "../window.h"

namespace smooth
{
	namespace GUI
	{
		class SMOOTHAPI WindowGDI : public WindowBase
		{
			private:
				static LRESULT CALLBACK		 WindowProc(HWND, UINT, WPARAM, LPARAM);
			protected:
				HICON				 sysIcon;
				HDC				 windowDC;

				Int				 origWndStyle;

				Bool				 Create();
			public:
				HWND				 hwnd;

								 WindowGDI(String title = NIL);
								~WindowGDI();

				Int				 SetIcon(const Bitmap &);

				Int				 SetApplicationIcon(char *);
				Int				 SetApplicationIcon(wchar_t *);

				Int				 SetMetrics(Point, Size);
				Int				 SetText(String);

				Int				 Show();
				Int				 Hide();

				Int				 Maximize();
				Int				 Restore();

				Int				 Stay();
				Int				 Close();

				Int				 Process(Int, Int, Int);

				Bool				 IsMouseOn(Rect);

				static WindowGDI		*GetWindow(HWND);
		};
	};
};

#endif
