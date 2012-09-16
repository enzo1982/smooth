 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_WINDOWGDI
#define H_OBJSMOOTH_WINDOWGDI

#include <windows.h>

namespace smooth
{
	namespace System
	{
		class Timer;
	};

	namespace GUI
	{
		class WindowGDI;
	};
};

#include "../windowbackend.h"
#include "../../../../graphics/forms/rect.h"
#include "../../../../input/keyboard.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 WINDOW_GDI	= 1;

		class WindowGDI : public WindowBackend
		{
			private:
				static LRESULT CALLBACK			 WindowProc(HWND, UINT, WPARAM, LPARAM);

				static Array<WindowGDI *, Void *>	 windowBackends;

				static WindowGDI			*GetWindowBackend(HWND);

				static System::Timer			*mouseNotifyTimer;

				Int					 ProcessSystemMessages(Int, Int, Int);

				WindowGDI				*FindLeaderWindow();
			protected:
				Int					 id;

				HWND					 hwnd;

				Void					*wndclass;
				String					 className;

				HICON					 sysIcon;
				Bool					 destroyIcon;

				Rect					 nonMaxRect;
				Int					 nonMaxWndStyle;

				Size					 frameSize;
				Float					 fontSize;

				Size					 minSize;
				Size					 maxSize;

				Bool					 minimized;
				Bool					 maximized;

				Int					 flags;

				Input::Keyboard::Key			 ConvertKey(Int);
			public:
									 WindowGDI(Void * = NIL);
									~WindowGDI();

				Void					*GetSystemWindow() const;

				Int					 Open(const String &, const Point &, const Size &, Int);
				Int					 Close();

				Int					 RequestClose();

				Int					 SetTitle(const String &);

				Int					 SetIcon(const Bitmap &);
				Int					 SetIconDirect(Void *);

				Int					 SetMinimumSize(const Size &);
				Int					 SetMaximumSize(const Size &);

				Int					 Show();
				Int					 Hide();

				Rect					 GetRestoredWindowRect() const;

				Int					 SetMetrics(const Point &, const Size &);

				Int					 Minimize();

				Int					 Maximize();
				Int					 Restore();

				Int					 Raise();

				static Void				 InitMouseNotifier();
				static Void				 FreeMouseNotifier();
			slots:
				static Void				 MouseNotifier();
		};
	};
};

#endif
